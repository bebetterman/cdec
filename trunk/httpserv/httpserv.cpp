// httpserv.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#pragma comment(lib, "cdecegg.lib")
#pragma comment(lib, "cdeccore.lib")
#pragma comment(lib, "../import-win/libmicrohttpd-0.9.17-w32/lib/libmicrohttpd.dll.a")

bool cdec_PrintError = true;

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// HandleContext
// -------------------------------------------------------------------------- //

int HandlerContext::SendResponse(UINT statusCode, const void* message, UINT length, bool fConstance)
{
	MHD_ResponseMemoryMode mm = fConstance ? MHD_RESPMEM_PERSISTENT : MHD_RESPMEM_MUST_COPY;
	struct MHD_Response * response;
	response = MHD_create_response_from_buffer(length, (void*)message, mm);
	int ret = MHD_queue_response(m_conn, statusCode, response);
	MHD_destroy_response(response);
	return ret;
}

void HandlerContext::Dispose()
{
	m_conn = NULL;
	m_postprocessor = NULL;
	m_method = HTTP_NONE;
	m_url = NULL;
	m_getArgs.clear();
	m_postArgs.clear();
}

// -------------------------------------------------------------------------- //
// DefaultUrlDispatcher
// -------------------------------------------------------------------------- //

ref<IRequestHandler> DefaultUrlDispatcher::Dispatch(stringx url)
{
	foreach_it (std::vector<Item>::iterator, it, m_items.begin(), m_items.end())
	{
		stringx pat = it->Url;
		size_t patLength = pat.Length();
		if (url.StartsWith(pat))
		{
			if (url.Length() == patLength || url[patLength] == '/')
				return it->Handler;
		}
	}
	return m_defaultHandler;
}

// -------------------------------------------------------------------------- //
// Server
// -------------------------------------------------------------------------- //

Server::Server(ref<IUrlDispatcher> dispatcher): m_daemon(NULL), m_dispatcher(dispatcher)
{
}

#ifndef X_OS_WINDOWS
#include <unistd.h>
#endif

void Server::Start(UINT port)
{
	UINT flags = MHD_USE_THREAD_PER_CONNECTION;
#ifdef DEBUG_HTTPCDEC
	flags |= MHD_USE_DEBUG;
#endif
#ifndef X_OS_WINDOWS
	flags |= MHD_USE_POLL;
#endif

	MHD_Daemon* d;
	d = MHD_start_daemon(
		flags, port, 
		NULL, NULL, 
		&OnRequestHandler, this,
		MHD_OPTION_NOTIFY_COMPLETED, &OnRequestCompleted, this,
		MHD_OPTION_END);
	if (d == NULL)
		cdec_throw(Exception(EC_Fail));

	while (true)
	{
		// Sleep for 10000 seconds
#ifdef X_OS_WINDOWS
		Sleep(10000 * 1000);
#else
		sleep(10000);
#endif
	// MHD_stop_daemon(d);
	}
}

int Server::OnParseGetArgs(void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
	HandlerContext* ctx = (HandlerContext*)cls;
	ref<Encoding> e = Encoding::get_UTF8();
	stringx key_s = e->ToUnicode(key);
	stringx value_s = value != NULL ? e->ToUnicode(value) : NULL;
	ctx->m_getArgs.insert(HandlerContext::Value(key_s, value_s));
	return MHD_YES;
}

int Server::OnPostDataIterator(void *cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size)
{
	ASSERT(kind == MHD_POSTDATA_KIND);
	HandlerContext* ctx = (HandlerContext*)cls;
	HttpCdecDebugLog("[POST ITERATOR]");
	HttpCdecDebugFormat("Key=%s\n", key);
	if (filename != NULL)
		HttpCdecDebugFormat("Filename=%s\n", filename);
	if (content_type != NULL)
		HttpCdecDebugFormat("MIME-type=%s\n", content_type);
	if (transfer_encoding != NULL)
		HttpCdecDebugFormat("Encoding=%s\n", transfer_encoding);
	
	std::string newvalue = std::string(data, size);
	HttpCdecDebugFormat("Value=%s\n", newvalue.c_str());

	ref<Encoding> e = Encoding::get_UTF8();
	stringx key_s = e->ToUnicode(key);

	HandlerContext::PostMap& map = ctx->m_postArgs;
	if (map.find(key_s) != map.end())
	{
		std::string& value = map[key_s];
		ASSERT(off == value.size());
		value += newvalue;
	}
	else
	{
		ASSERT(off == 0);
		map.insert(HandlerContext::PostMap::value_type(key_s, newvalue));
	}

	return MHD_YES;
}

int Server::OnRequestHandler(void* hdctx, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** reqctx)
{
	Server* server = (Server*)hdctx;

	if (*reqctx == NULL)
	{
		ASSERT(upload_data == NULL && *upload_data_size == 0);
		ref<HandlerContext> ctx = gc_new<HandlerContext>();
		ctx->m_conn = connection;
		ctx->m_url = Encoding::get_UTF8()->ToUnicode(url);

		if (strcmp(method, MHD_HTTP_METHOD_GET) == 0)
		{
			ctx->m_method = HandlerContext::HTTP_GET;
			ctx->m_postprocessor = NULL;
		}
		else if (strcmp(method, MHD_HTTP_METHOD_POST) == 0)
		{
			ctx->m_method = HandlerContext::HTTP_POST;
			ctx->m_postprocessor = MHD_create_post_processor(connection, 1024, OnPostDataIterator, ctx.__GetPointer());
		}
		else
			return MHD_NO;

		// The first time only the headers are valid, do not respond in the first round.
		*reqctx = ctx.__GetPointer();
		ctx->AddRef();	// interop: hold the cdec object
		return MHD_YES;
	}
	else
	{
		HandlerContext* ctx = (HandlerContext*)(*reqctx);

		// Parse GET arguments
		MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, OnParseGetArgs, ctx);

		// Parse headers
		// MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);

		ref<IRequestHandler> handler = server->m_dispatcher->Dispatch(ctx->m_url);

		if (ctx->m_method == HandlerContext::HTTP_GET)
		{
			ASSERT(strcmp(method, MHD_HTTP_METHOD_GET) == 0);
			ASSERT(*upload_data_size == 0);

			// Do not call "*reqctx = NULL;" because a request completed callback would be called instead
			return handler->Handle(ctx);
		}
		else if (ctx->m_method == HandlerContext::HTTP_POST)
		{
			if (*upload_data_size != 0)
			{
				MHD_post_process (ctx->m_postprocessor, upload_data, *upload_data_size);
				*upload_data_size = 0;
          
				return MHD_YES;
			}
			else
			{
				HttpCdecDebugLog("[POST COMPLETE]");

				// Do not call "*reqctx = NULL;" because a request completed callback would be called instead
				return handler->Handle(ctx);
			}
		}
		else
			return MHD_NO;
	}
}

void Server::OnRequestCompleted(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe)
{
	Server* server = (Server*)cls;
	HandlerContext* ctx = (HandlerContext*)*con_cls;

	if (ctx->m_postprocessor != NULL)
	{
		puts("[POST REQUEST COMPLETED]");
		ASSERT(ctx->m_method == HandlerContext::HTTP_POST);
		MHD_destroy_post_processor(ctx->m_postprocessor);
	}
	else
	{
		ASSERT(ctx->m_method != HandlerContext::HTTP_POST);
	}

	ctx->Dispose();
	ctx->Release();		// interop: release the object held
	*con_cls = NULL;   
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
