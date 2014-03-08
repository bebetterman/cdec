// TestHttpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "cdecegg.lib")
#pragma comment(lib, "cdeccore.lib")
#pragma comment(lib, "httpserv.lib")
#pragma comment(lib, "../../import-win/libmicrohttpd-0.9.17-w32/lib/libmicrohttpd.dll.a")

using namespace cdec;

const char MESSAGE_404_NOT_FOUND[] = "404 File not found";

const char MESSAGE_EXPECT_POST[] = "POST method expected";

class NotFoundHandler: public IRequestHandler
{
	DECLARE_REF_CLASS(NotFoundHandler)

public:
	int Handle(ref<HandlerContext> ctx)
	{
		return ctx->SendResponse(MHD_HTTP_NOT_FOUND, MESSAGE_404_NOT_FOUND, sizeof(MESSAGE_404_NOT_FOUND), true);
	}
};

class MyGetHandler: public IRequestHandler
{
	DECLARE_REF_CLASS(MyGetHandler)

public:
	int Handle(ref<HandlerContext> ctx)
	{
		ASSERT(ctx->GetMethod() == HandlerContext::HTTP_GET);
		ref<StringBuilder> sb = gc_new<StringBuilder>();
		sb->Append(__X("<html><body>\n<p>POST</p>\n"));
		sb->Append(__X("<p>URL=") + ctx->GetUrl() + __X("</p>\n"));

		sb->Append(__X("<p>Headers</p>\n"));
		OutputKeyValueMap(ctx->GetHeaders(), sb);

		sb->Append(__X("<p>GET arguments</p>\n"));
		OutputKeyValueMap(ctx->GetArgs(), sb);

		sb->Append(__X("</body></html>\n"));
		return ctx->SendResponse(MHD_HTTP_OK, sb);
	}

	static void OutputKeyValueMap(const HandlerContext::StringPairMap& map, ref<StringBuilder> sb)
	{
		sb->Append(__X("<ul>\n"));
		foreach_it (HandlerContext::StringPairMap::const_iterator, it, map.begin(), map.end())
		{
			sb->Append(__X("<li>"));
			sb->Append(it->first);
			sb->Append('=');
			sb->Append(it->second != NULL ? it->second : __X("(null)"));
			sb->Append(__X("</li>\n"));
		}
		sb->Append(__X("</ul>\n"));
	}
};

class MyPostHandler: public IRequestHandler
{
	DECLARE_REF_CLASS(MyGetHandler)

public:
	int Handle(ref<HandlerContext> ctx)
	{
		ref<Encoding> e = Encoding::get_UTF8();

		if (ctx->GetMethod() != HandlerContext::HTTP_POST)
			return ctx->SendResponse(MHD_HTTP_OK, MESSAGE_EXPECT_POST, sizeof(MESSAGE_EXPECT_POST), true);

		ref<StringBuilder> sb = gc_new<StringBuilder>();
		sb->Append(__X("<html><body>\n<p>POST</p>\n"));
		sb->Append(__X("<p>URL=") + ctx->GetUrl() + __X("</p>\n"));

		sb->Append(__X("<p>Headers</p>\n"));
		MyGetHandler::OutputKeyValueMap(ctx->GetHeaders(), sb);

		sb->Append(__X("<p>GET arguments</p>\n"));
		MyGetHandler::OutputKeyValueMap(ctx->GetArgs(), sb);

		sb->Append(__X("<p>POST arguments</p>\n<ul>\n"));
		const HandlerContext::PostMap& args = ctx->PostArgs();
		foreach_it (HandlerContext::PostMap::const_iterator, it, args.begin(), args.end())
		{
			sb->Append(__X("<li>"));
			sb->Append(it->first);
			sb->Append('=');
			sb->Append(e->ToUnicode(it->second));
			sb->Append(__X("</li>\n"));
		}
		sb->Append(__X("</ul>\n"));
		
		sb->Append(__X("</body></html>\n"));
		return ctx->SendResponse(MHD_HTTP_OK, sb);
	}
};

class MyRawPostHandler: public IRequestHandler
{
	DECLARE_REF_CLASS(MyGetHandler)

public:
	int Handle(ref<HandlerContext> ctx)
	{
		ref<Encoding> e = Encoding::get_UTF8();

		if (ctx->GetMethod() != HandlerContext::HTTP_POST)
			return ctx->SendResponse(MHD_HTTP_OK, MESSAGE_EXPECT_POST, sizeof(MESSAGE_EXPECT_POST), true);

		ref<StringBuilder> sb = gc_new<StringBuilder>();
		sb->Append(__X("<html><body>\n<p>POST</p>\n"));
		sb->Append(__X("<p>URL=") + ctx->GetUrl() + __X("</p>\n"));

		sb->Append(__X("<p>Headers</p>\n"));
		MyGetHandler::OutputKeyValueMap(ctx->GetHeaders(), sb);

		sb->Append(__X("<p>GET arguments</p>\n"));
		MyGetHandler::OutputKeyValueMap(ctx->GetArgs(), sb);

		sb->Append(__X("<p>POST data</p>\n<p>\n"));
		sb->Append(Encoding::get_UTF8()->GetString(ctx->PostData()));
		sb->Append(__X("</p>\n"));

		sb->Append(__X("</body></html>\n"));
		return ctx->SendResponse(MHD_HTTP_OK, sb);
	}

	bool KeepPostData() { return true; }
};

int main(int argc, const char* argv[])
{
	ref<DefaultUrlDispatcher> dispatcher = 
		gc_new<DefaultUrlDispatcher>(gc_new<NotFoundHandler>());
	dispatcher->Add(__X("/"), gc_new<StaticHandler>("Default Page"));
	dispatcher->Add(__X("/hello"), gc_new<StaticHandler>("Hello Page"));
	dispatcher->Add(__X("/get"), gc_new<MyGetHandler>());
	dispatcher->Add(__X("/post"), gc_new<MyPostHandler>());
	dispatcher->Add(__X("/rawpost"), gc_new<MyRawPostHandler>());

	ref<Server> server = gc_new<Server>(dispatcher);
	server->Start(6001);

	return 0;
}
