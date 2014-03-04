#include "stdafx.h"

#pragma comment(lib, "cdecegg")
#pragma comment(lib, "cdeccore")
#pragma comment(lib, "curlcdec")

bool cdec_PrintError = true;

#define MYURL	"http://localhost:6001/"

#define UNITTEST_ASSERT	ASSERT

// -------------------------------------------------------------------------- //

void TestGetPage()
{
	Console::WriteLine(__X("TestGetPage"));
	ref<CurlEasy> curl = gc_new<CurlEasy>();
	curl->SetUrl(MYURL "get?name=abc&value=123");
	curl->SetTimeOut(30);

	try
	{
		ref<CurlResponse> response = curl->Request();
		UNITTEST_ASSERT(response->GetResponseCode() == 200);
		stringx s = response->GetString();
		Console::WriteLine(s);
	}
	catch (Exception e)
	{
		if (e.Code == EC_CURL_OPERATION_TIMEDOUT)
			Console::WriteLine(__X("Request time out"));
		else
			Console::WriteLine(__X("Exception code ") + Converter::ToString(e.Code));
	}
}

void TestPostPage()
{
	Console::WriteLine(__X("TestPost"));
	ref<CurlEasy> curl = gc_new<CurlEasy>();
	curl->SetUrl(MYURL "post");
	curl->SetPostText(__X("name=abc&value=123"));
	ref<CurlResponse> response = curl->Request();

	UNITTEST_ASSERT(response->GetResponseCode() == 200);
	stringx s = response->GetString();
	Console::WriteLine(s);
}

void TestRawPostPage()
{
	Console::WriteLine(__X("TestRawPost"));
	ref<CurlEasy> curl = gc_new<CurlEasy>();
	curl->SetUrl(MYURL "rawpost");
	curl->AddHeader(__X("Content-Type"), __X("text/plain"));
	curl->SetPostText(__X("name=abc&value=123"));
	ref<CurlResponse> response = curl->Request();

	UNITTEST_ASSERT(response->GetResponseCode() == 200);
	stringx s = response->GetString();
	Console::WriteLine(s);
}

void TestReadResponseHeaders()
{
	Console::WriteLine(__X("TestReadResponseHeaders"));
	ref<CurlEasy> curl = gc_new<CurlEasy>(CCO_ResponseHeaders);
	curl->SetUrl("http://www.google.com/");
	ref<CurlResponse> response = curl->Request();

	Console::WriteLine(__X("Response code: ") + Converter::ToString(response->GetResponseCode()));
	Console::WriteLine(__X("State: ") + response->GetHttpState());
	ref<CurlResponse::HeaderMap> headers = response->GetHeaders();
	foreach (CurlResponse::HeaderMap::KeyValuePair, pair, headers)
		Console::WriteLine('\"' + pair.first + __X("\" : \"") + pair.second + '\"');
}

int main(int argc, char* argv[])
{
	CurlEasy::GlobalInit();

	// Run TestHttpServ before launching this test
	TestGetPage();
	TestPostPage();
	TestRawPostPage();
	TestReadResponseHeaders();

	CurlEasy::GlobalTerm();
	return 0;
}
