// -------------------------------------------------------------------------- //
// Microhttpd Simple Test 03
//
// A HTTP server from which to download files in remote file system
//
// Works under Linux but does not work under Windows
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
#include <map>
#include <string>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <sys/stat.h>

// -------------------------------------------------------------------------- //

const char BAD_REQUEST_ERROR[] = "<html><head><title>Bad request</title></head><body>Bad Request.</body></html>";
const char NOT_FOUND_ERROR[] = "<html><head><title>Not found</title></head><body>Not Found.</body></html>";

struct ServerContext
{
	// Not used yet
};

typedef std::map<std::string, std::string> KeyValueMap;

int print_out_key(void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
	KeyValueMap* map = (KeyValueMap*)cls;
	map->insert(KeyValueMap::value_type(key, value));
	return MHD_YES;
}

int SendStaticMessage(MHD_Connection* connection, unsigned int statusCode, const char* message, int length)
{
	MHD_Response* response = MHD_create_response_from_buffer(length, (void*)message, MHD_RESPMEM_PERSISTENT);
	MHD_add_response_header(response, "Content-Type", "text/html");
	int ret = MHD_queue_response(connection, statusCode, response);
	MHD_destroy_response(response);
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
 
		// Parse GET parameters
		KeyValueMap args;
		MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, &print_out_key, &args);

		if (args.find("tag") == args.end())
		{
			*reqctx = NULL; // clear context pointer
			return SendStaticMessage(connection, MHD_HTTP_BAD_REQUEST, BAD_REQUEST_ERROR, sizeof(BAD_REQUEST_ERROR));
		}

		std::string path = args["tag"];
		printf("Download %s\n", path.c_str());
		int fd;
		struct stat stat_v;
		if ((-1 == (fd = open(path.c_str(), O_RDONLY))) || (0 != fstat(fd, &stat_v)))
		{
			if (fd != -1)
				close(fd);
			*reqctx = NULL; // clear context pointer
			return SendStaticMessage(connection, MHD_HTTP_NOT_FOUND, NOT_FOUND_ERROR, sizeof(NOT_FOUND_ERROR));
		}

		MHD_Response* response = MHD_create_response_from_fd_at_offset(stat_v.st_size, fd, 0);
		MHD_add_response_header(response, "Content-Type", "image/jpeg");
		int ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
		MHD_destroy_response (response);

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
