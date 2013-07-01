// -------------------------------------------------------------------------- //
// Microhttpd Simple Test 05
//
// POST data accepted
//
// -------------------------------------------------------------------------- //

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winsock2.h>

#include <xutility>

typedef SSIZE_T ssize_t;
typedef int socklen_t;

typedef UINT16 uint16_t;
typedef UINT64 uint64_t;

#define MHD_PLATFORM_H
#include "../../import-win/libmicrohttpd-0.9.17-w32/include/microhttpd.h"
#pragma comment(lib, "../../import-win/libmicrohttpd-0.9.17-w32/lib/libmicrohttpd.dll.a")

#else

#include "microhttpd.h"

#include <stdio.h>
#include <string.h>

#endif

#include <assert.h>
#include <string>
#include <map>

// -------------------------------------------------------------------------- //

const char HELLO_MESSAGE[] = "<html><body><p>POST method expected</p></body></html>\n";

struct ServerContext
{
	int	MagicNumber;
};

enum HttpMethod
{
	HTTPMETHOD_GET,
	HTTPMETHOD_POST,
};

struct ConnContext
{
	int	Method;
	MHD_PostProcessor *PostProcessor;
	std::map<std::string, std::string> KeyValueMap;
};

int MyPostDataIterator(void *cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size)
{
	assert(kind == MHD_POSTDATA_KIND);
	ConnContext* conctx = (ConnContext*)cls;
	puts("[POST ITERATOR]");
	printf("Key=%s\n", key);
	if (filename != NULL)
		printf("Filename=%s\n", filename);
	if (content_type != NULL)
		printf("MIME-type=%s\n", content_type);
	if (transfer_encoding != NULL)
		printf("Encoding=%s\n", transfer_encoding);
	
	std::string newvalue = std::string(data, size);
	printf("Value=%s\n", newvalue.c_str());

	std::map<std::string, std::string>& map = conctx->KeyValueMap;
	if (map.find(key) != map.end())
	{
		std::string& value = map[key];
		assert(off == value.size());
		value += newvalue;
	}
	else
	{
		assert(off == 0);
		map.insert(std::map<std::string, std::string>::value_type(key, newvalue));
	}

	return MHD_YES;
}

void MyRequestCompleted(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe)
{
	ServerContext* servctx_v = (ServerContext*)cls;
	assert(servctx_v->MagicNumber == 0xbabeface);

	ConnContext* conctx = (ConnContext*)*con_cls;
	if (conctx->PostProcessor != NULL)
	{
		puts("[POST REQUEST COMPLETED]");
		assert(conctx->Method == HTTPMETHOD_POST);
		MHD_destroy_post_processor(conctx->PostProcessor);
		conctx->KeyValueMap.clear();
	}
	else
	{
		assert(conctx->Method != HTTPMETHOD_POST);
	}

	delete conctx;
	*con_cls = NULL;   
}

int ahc_echo(void* servctx, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** reqctx)
{
	ServerContext* servctx_v = (ServerContext*)servctx;
	assert(servctx_v->MagicNumber == 0xbabeface);

	if (*reqctx == NULL)
	{
		assert(upload_data == NULL && *upload_data_size == 0);
		ConnContext* conctx = new ConnContext();
		if (strcmp(method, MHD_HTTP_METHOD_GET) == 0)
		{
			conctx->Method = HTTPMETHOD_GET;
			conctx->PostProcessor = NULL;
		}
		else if (strcmp(method, MHD_HTTP_METHOD_POST) == 0)
		{
			conctx->Method = HTTPMETHOD_POST;
			conctx->PostProcessor = MHD_create_post_processor(connection, 1024, MyPostDataIterator, conctx);
		}
		else
			return MHD_NO;		

		// The first time only the headers are valid, do not respond in the first round.
		*reqctx = conctx;
		return MHD_YES;
	}
	else
	{
		ConnContext* conctx = (ConnContext*)(*reqctx);
		if (conctx->Method == HTTPMETHOD_GET)
		{
			assert(strcmp(method, MHD_HTTP_METHOD_GET) == 0);
			assert(*upload_data_size == 0);

			MHD_Response* response = MHD_create_response_from_buffer(sizeof(HELLO_MESSAGE), (void*)HELLO_MESSAGE, MHD_RESPMEM_PERSISTENT);
			MHD_add_response_header(response, "Content-Type", "text/html");
			int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
			MHD_destroy_response(response);

			// Do not call "*reqctx = NULL;" because a request completed callback would be called instead
			return ret;
		}
		else if (conctx->Method == HTTPMETHOD_POST)
		{
			if (*upload_data_size != 0)
			{
				MHD_post_process (conctx->PostProcessor, upload_data, *upload_data_size);
				*upload_data_size = 0;
          
				return MHD_YES;
			}
			else
			{
				puts("[POST COMPLETE]");

				std::string message = "<html><body>\n<p>POST</p>\n<ul>\n";
				for (std::map<std::string, std::string>::iterator it = conctx->KeyValueMap.begin(); it != conctx->KeyValueMap.end(); ++it)
				{
					message += "<li>"; message += it->first; message += '='; message += it->second;
					message += "</li>\n";
				}
				message += "</ul>\n</body></html>\n";

				MHD_Response* response = MHD_create_response_from_buffer(message.size(), (void*)message.c_str(), MHD_RESPMEM_MUST_COPY);
				MHD_add_response_header(response, "Content-Type", "text/html");
				int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
				MHD_destroy_response(response);

				// Do not call "*reqctx = NULL;" because a request completed callback would be called instead
				return ret;
			}
		}
		else
			return MHD_NO;
	}
}

// -------------------------------------------------------------------------- //

int main(int argc, char* argv[])
{
	ServerContext servctx;	// Not used
	servctx.MagicNumber = 0xbabeface;
	MHD_Daemon* d;
	d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, 80, NULL, NULL, &ahc_echo, &servctx, 
		MHD_OPTION_NOTIFY_COMPLETED, MyRequestCompleted, &servctx, MHD_OPTION_END);
	if (d != NULL)
	{
		puts("Press enter key.");
		getchar();
		MHD_stop_daemon(d);
	}
	else
		puts("Failed to start server.");

	return 0;
}
