#include "stdafx.h"

// -------------------------------------------------------------------------- //

#ifdef _WIN32
#	ifdef _DEBUG
#		pragma comment(lib, "libcurl_vc10_static_debug")
#	endif
#endif

struct CurlException
{
	int code;
	CurlException(int _code): code(_code) {}
};

#define MYURL	"http://localhost:6001/"

#define VERIFY(f) UNITTEST_ASSERT((f) == 0)

void Initialize()
{
#if defined(_WIN32)
	long flags = CURL_GLOBAL_WIN32;	
#else
	long flags = CURL_GLOBAL_NOTHING;
#endif

	int code = curl_global_init(flags);
	UNITTEST_ASSERT(code == 0);
}

void Terminate()
{
	curl_global_cleanup();
}

// -------------------------------------------------------------------------- //

typedef std::string ByteBuffer;

size_t CurlDataReceiveCallback(void *buffer, size_t size, size_t nmemb, void *user_p)
{
	ByteBuffer* p = (ByteBuffer*)user_p;
	size_t length = size * nmemb;
	p->append((const char*)buffer, size * nmemb);
	return length;
}

// -------------------------------------------------------------------------- //

void TestGet()
{
	puts("[TestGet]");

	ByteBuffer buffer;

	void* curl = curl_easy_init();
	UNITTEST_ASSERT(curl != NULL);

	VERIFY(curl_easy_setopt(curl, CURLOPT_URL, MYURL "get?name=abc&value=123"));
	VERIFY(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlDataReceiveCallback));
	VERIFY(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer));
	VERIFY(curl_easy_perform(curl));

	long responseCode = 0;
	VERIFY(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode));
	UNITTEST_ASSERT(responseCode == 200);
	
	curl_easy_cleanup(curl);

	puts(buffer.c_str());
}

void TestPost()
{
	puts("[TestPost]");

	ByteBuffer buffer;

	void* curl = curl_easy_init();
	UNITTEST_ASSERT(curl != NULL);

	VERIFY(curl_easy_setopt(curl, CURLOPT_URL, MYURL "post"));	// Set URL
	VERIFY(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=abc&value=123"));	// Set post text
	VERIFY(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlDataReceiveCallback));
	VERIFY(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer));
	VERIFY(curl_easy_perform(curl));

	long responseCode = 0;
	VERIFY(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode));
	UNITTEST_ASSERT(responseCode == 200);
	
	curl_easy_cleanup(curl);

	puts(buffer.c_str());
}

void TestPostBinary()
{
	puts("[TestPost]");

	ByteBuffer buffer;

	void* curl = curl_easy_init();
	UNITTEST_ASSERT(curl != NULL);

	VERIFY(curl_easy_setopt(curl, CURLOPT_URL, MYURL "rawpost"));
	
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: text/xml");		// Set header Content-Type
	VERIFY(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers));

	const char postText[] = "<document><name>abc</name></document>";
	VERIFY(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postText));		// Set post data
	VERIFY(curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sizeof(postText) - 1));	// Set post data length

	VERIFY(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlDataReceiveCallback));
	VERIFY(curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer));
	VERIFY(curl_easy_perform(curl));

	long responseCode = 0;
	VERIFY(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode));
	UNITTEST_ASSERT(responseCode == 200);

	curl_slist_free_all(headers); // free the header list
	curl_easy_cleanup(curl);

	puts(buffer.c_str());
}

// -------------------------------------------------------------------------- //

int main(int argc, char* argv[])
{
	Initialize();

	TestGet();
	TestPost();
	TestPostBinary();

	Terminate();

	return 0;
}
