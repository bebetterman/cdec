// -------------------------------------------------------------------------- //
// Microhttpd Simple Test 04
//
// A HTTP server from which to download files in remote file system
//
// Use iterator function to provider file data
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
#include <algorithm>
#include <map>
#include <string>
#include <sys/stat.h>

// -------------------------------------------------------------------------- //

const char NORMAL_PAGE[] = "<html><body><p>Use URL like this: http://localhost/get/?tag=ImagePath</p></body></html>";
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
	return ret;
}

ssize_t FileContentReaderCallback(void *cls, uint64_t pos, char *buf, size_t max)
{
	FILE* fp = (FILE*)cls;
	fseek(fp, pos, SEEK_SET);
	return fread(buf, 1, max, fp);
}

void FileContentReaderFreeCallback(void *cls)
{
	fclose((FILE*)cls);
}

std::string GetFileName(std::string path)
{
	size_t pos = path.find_last_of("\\/");
	if (pos != std::string::npos)
		return path.substr(pos + 1);
	else
		return path;
}

std::string GetFileExt(std::string path)
{
	size_t pos = path.find_last_of(".\\/");
	if (pos == std::string::npos)
		return std::string();
	if (path[pos] == '.')
		return path.substr(pos);
	else
		return std::string();
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
		if (strncmp(url, "/get", 4) != 0 || args.find("tag") == args.end())
		{
			*reqctx = NULL; // clear context pointer
			return SendStaticMessage(connection, MHD_HTTP_OK, NORMAL_PAGE, sizeof(NORMAL_PAGE));
		}

		std::string path = args["tag"];
		FILE* fp = fopen(path.c_str(), "rb");
		if (fp == NULL)
		{
			*reqctx = NULL; // clear context pointer
			return SendStaticMessage(connection, MHD_HTTP_NOT_FOUND, NOT_FOUND_ERROR, sizeof(NOT_FOUND_ERROR));
		}

		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		MHD_Response* response = MHD_create_response_from_callback(fsize, 0x4000, FileContentReaderCallback, fp, FileContentReaderFreeCallback);
		std::string ext = GetFileExt(path);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		if (ext == ".jpg" || ext == ".jpeg")
			MHD_add_response_header(response, "Content-Type", "image/jpeg");
		else
		{
			std::string disposition = "attachment; filename=\"";
			disposition += GetFileName(path);
			disposition += "\";";
			MHD_add_response_header(response, "Content-Disposition", disposition.c_str());
		}
		int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
		MHD_destroy_response(response);

		// Do not call fclose for this would be done by FileContentReaderFreeCallback call back function

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
