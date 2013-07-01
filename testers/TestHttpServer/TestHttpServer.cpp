// TestHttpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "cdeccore.lib")
#pragma comment(lib, "httpserv.lib")
#pragma comment(lib, "../../import-win/libmicrohttpd-0.9.17-w32/lib/libmicrohttpd.dll.a")

using namespace cdec;

const char MESSAGE_404_NOT_FOUND[] = "404 File not found";

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
		ref<StringBuilder> sb = gc_new<StringBuilder>();
		sb->Append(__X("URL=") + ctx->GetUrl());
		sb->Append('\n');

		const HandlerContext::Values& args = ctx->GetArgs();
		foreach_it (HandlerContext::Values::const_iterator, it, args.begin(), args.end())
		{
			sb->Append(it->first);
			sb->Append('=');
			if (it->second != NULL)
			{
				sb->Append(it->second);
				sb->Append('\n');
			}
			else
				sb->Append(__X("(null)\n"));
		}

		return ctx->SendResponse(MHD_HTTP_OK, sb);
	}
};

int main(int argc, const char* argv[])
{
	ref<DefaultUrlDispatcher> dispatcher = 
		gc_new<DefaultUrlDispatcher>(gc_new<NotFoundHandler>());
	dispatcher->Add(__X("/"), gc_new<StaticHandler>("Default Page"));
	dispatcher->Add(__X("/hello"), gc_new<StaticHandler>("Hello Page"));
	dispatcher->Add(__X("/get"), gc_new<MyGetHandler>());

	ref<Server> server = gc_new<Server>(dispatcher);
	server->Start(6001);

	return 0;
}
