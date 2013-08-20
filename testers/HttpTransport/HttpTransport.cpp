#include "../../include/cdeccore.h"
#include "../../include/curlcdec.h"
#include "../../include/httpserv.h"

using namespace cdec;

#ifdef X_OS_WINDOWS
#	pragma comment(lib, "cdecegg")
#	pragma comment(lib, "cdeccore")
#	pragma comment(lib, "curlcdec")
#	pragma comment(lib, "httpserv")
#endif

// -------------------------------------------------------------------------- //

class TransportHandler: public IRequestHandler
{
	DECLARE_REF_CLASS(TransportHandler)

protected:
	stringx	m_baseUrl;

public:
	TransportHandler(stringx baseUrl): m_baseUrl(baseUrl) {}

	bool KeepPostData() { return true; }
	int Handle(ref<HandlerContext> ctx);
};

// -------------------------------------------------------------------------- //

int TransportHandler::Handle(ref<HandlerContext> ctx)
{
	stringx url = ctx->GetUrl();

	const HandlerContext::StringPairMap& args = ctx->GetArgs();
	const HandlerContext::StringPairMap& headers = ctx->GetHeaders();
	ref<ByteArray> data = NULL;
	if (ctx->GetMethod() == HandlerContext::HTTP_POST)
		data = ctx->PostData();

	stringx newurl = m_baseUrl + url;

	if (args.size() != 0)
	{
		bool first = true;
		foreach_it (HandlerContext::StringPairMap::const_iterator, it, args.begin(), args.end())
		{
			newurl += (first ? '?' : '&') + it->first + '=' + it->second;
			first = false;
		}
	}

	ref<CurlEasy> curl = gc_new<CurlEasy>();
	curl->SetUrl(newurl);

	// [TODO] Some headers doesn't work for transport a web browser request
//	foreach_it (HandlerContext::StringPairMap::const_iterator, it, headers.begin(), headers.end())
//		curl->AddHeader(it->first, it->second);

	if (data != NULL)
		curl->SetPostBytes(data);

	curl->Request();

	int httpcode = curl->GetResponseCode();

	data = curl->ReadResponseData();
	if(data->Count() >= 1)
	{
		ref<Encoding> e = Encoding::get_UTF8();
		stringx dataStr = e->GetString(data,0,data->Count());
	}
	return ctx->SendResponse(httpcode, data, 0, data->Count());
}

// -------------------------------------------------------------------------- //

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		puts("Arguments: port source\neg. HttpTransport 6009 http://172.16.2.202:6009");
		return 1;
	}

	int port = atoi(argv[1]);
	PCSTR source = argv[2];
	printf("Source = %s, Port = %d\n", source, port);

	stringx source_s = Encoding::get_UTF8()->ToUnicode(source);
	ref<DefaultUrlDispatcher> dispatcher = gc_new<DefaultUrlDispatcher>(gc_new<TransportHandler>(source_s));
	ref<Server> server = gc_new<Server>(dispatcher);
	server->Start(port);

	return 0;
}
