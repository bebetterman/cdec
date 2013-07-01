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

stringx HandlerContext::GetArg(stringx key)
{
	return m_args[key];
}

int HandlerContext::SendResponse(UINT statusCode, const void* message, UINT length, bool fConstance)
{
	MHD_ResponseMemoryMode mm = fConstance ? MHD_RESPMEM_PERSISTENT : MHD_RESPMEM_MUST_COPY;
	struct MHD_Response * response;
	response = MHD_create_response_from_buffer(length, (void*)message, mm);
	int ret = MHD_queue_response(m_conn, statusCode, response);
	MHD_destroy_response(response);
	return ret;
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
	UINT flags = MHD_USE_THREAD_PER_CONNECTION | MHD_USE_POLL;
	// UINT flags = MHD_USE_THREAD_PER_CONNECTION | MHD_USE_POLL | MHD_USE_DEBUG;
	MHD_Daemon* d;
	d = MHD_start_daemon(flags, port, NULL, NULL, &ahc_echo, this, MHD_OPTION_END);
	if (d == NULL)
		cdec_throw(Exception(EC_Fail));
	// getchar();
	// MHD_stop_daemon(d);
	while (true)
	{
		// Sleep for 10000 seconds
#ifdef X_OS_WINDOWS
		Sleep(10000 * 1000);
#else
		sleep(10000);
#endif
	}
}

int Server::parse_get_args(void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
	HandlerContext* ctx = (HandlerContext*)cls;
	ref<Encoding> e = Encoding::get_UTF8();
	stringx key_s = e->ToUnicode(key);
	stringx value_s = value != NULL ? e->ToUnicode(value) : NULL;
	ctx->m_args.insert(HandlerContext::Value(key_s, value_s));
	return MHD_YES;
}

int Server::ahc_echo(void* hdctx, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** ptr)
{
	static int dummy;
	Server* server = (Server*)hdctx;

	if (&dummy != *ptr)
	{
		// The first time only the headers are valid, do not respond in the first round.
		*ptr = &dummy;
		return MHD_YES;
	}

	ref<HandlerContext> ctx = gc_new<HandlerContext>();
	ctx->m_conn = connection;
	ctx->m_url = Encoding::get_UTF8()->ToUnicode(url);

	if (0 == strcmp(method, "GET"))
	{
		ctx->m_method = HandlerContext::HTTP_GET;
		if (*upload_data_size != 0)
			return MHD_NO;	// No upload data expected in HTTP GET method
	}
	else if (0 == strcmp(method, "POST"))
	{
		ctx->m_method = HandlerContext::HTTP_POST;
		if (*upload_data_size == 0)
			return MHD_NO;	// Upload data expected in HTTP POST method
	}
	else
		return MHD_NO; // Unexpected method

	// Parse GET arguments
	MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, parse_get_args, ctx.__GetPointer());

	// Parse headers
	// MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);

	*ptr = NULL; // clear context pointer

	ref<IRequestHandler> handler = server->m_dispatcher->Dispatch(ctx->m_url);
	return handler->Handle(ctx);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
