// -------------------------------------------------------------------------- //
// Microhttpd Simple Test 02
//
// Request string is parsed as key-value pairs
// Headers are printed
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

#endif

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

// -------------------------------------------------------------------------- //

struct ServerContext
{
	// Not used yet
};

typedef std::vector<std::pair<std::string, std::string> > StringPairList;

int print_out_key(void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
	StringPairList* spl = (StringPairList*)cls;
	printf("%s: %s\n", key, value);
	if (spl != NULL)
		spl->push_back(StringPairList::value_type(key, value));
	return MHD_YES;
}

int ahc_echo(void* servctx, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** reqctx)
{
	static int dummy;
	if (reqctx == NULL)
	{
		// The first time only the headers are valid, do not respond in the first round.
		*reqctx = &dummy;
		return MHD_YES;
	}

	ServerContext* pServCtx = (ServerContext*)servctx;	// Not used yet
	printf("%s %s %s\n", method, url, version);

	if (strcmp(method, "GET") == 0)
	{
		if (*upload_data_size != 0)
			return MHD_NO;	// No upload data expected in HTTP GET method
 
		// Parse headers
		puts("[HEADERS]");
		MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);

		// Parse GET parameters
		puts("[GET PARAMS]");
		StringPairList spl;
		MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, &print_out_key, &spl);

		std::string text = "<html><body>\n";
		text += "<p>URL="; text += url; 
		text += "</p>\n<p>Parameters</p>\n<ul>\n";
		for (StringPairList::iterator it = spl.begin(); it != spl.end(); ++it)
		{
			text += "<li>"; text += it->first; text += '='; text += it->second; text += "</li>\n";
		}
		text += "</body></html>\n";

		MHD_Response* response = MHD_create_response_from_buffer(text.size(), (void*)text.c_str(), MHD_RESPMEM_MUST_COPY);
		MHD_add_response_header(response, "Content-Type", "text/html");
		int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
		MHD_destroy_response(response);

		*reqctx = NULL; // clear context pointer
		return ret;
	}
	else
		return MHD_NO;	// Not supported yet
}

// -------------------------------------------------------------------------- //

int main(int argc, char* argv[])
{
	ServerContext servctx;	// Not used
	MHD_Daemon* d;
	d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, 80, NULL, NULL, &ahc_echo, &servctx, MHD_OPTION_END);
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
