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
	curl->Request();

	UNITTEST_ASSERT(curl->GetResponseCode() == 200);
	ref<ByteArray> data = curl->ReadResponseData();
	stringx s = Encoding::get_UTF8()->GetString(data);
	Console::WriteLine(s);
}

void TestPostPage()
{
	Console::WriteLine(__X("TestPost"));
	ref<CurlEasy> curl = gc_new<CurlEasy>();
	curl->SetUrl(MYURL "post");
	curl->SetPostText(__X("name=abc&value=123"));
	curl->Request();

	UNITTEST_ASSERT(curl->GetResponseCode() == 200);
	ref<ByteArray> data = curl->ReadResponseData();
	stringx s = Encoding::get_UTF8()->GetString(data);
	Console::WriteLine(s);
}

void TestRawPostPage()
{
	Console::WriteLine(__X("TestRawPost"));
	ref<CurlEasy> curl = gc_new<CurlEasy>();
	curl->SetUrl(MYURL "rawpost");
	curl->AddHeader(__X("Content-Type"), __X("text/plain"));
	curl->SetPostText(__X("name=abc&value=123"));
	curl->Request();

	UNITTEST_ASSERT(curl->GetResponseCode() == 200);
	ref<ByteArray> data = curl->ReadResponseData();
	stringx s = Encoding::get_UTF8()->GetString(data);
	Console::WriteLine(s);
}

int main(int argc, char* argv[])
{
	CurlEasy::GlobalInit();

	// Run TestHttpServ before launching this test
	TestGetPage();
	TestPostPage();
	TestRawPostPage();

	CurlEasy::GlobalTerm();
	return 0;
}
