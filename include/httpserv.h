#pragma once

// -------------------------------------------------------------------------- //
// Project dependency
// -------------------------------------------------------------------------- //

#ifndef __CDECCORE__
#error Import cdeccore first
#endif

#ifdef X_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winsock2.h>

typedef SSIZE_T ssize_t;
typedef int socklen_t;

typedef UINT16 uint16_t;
typedef UINT64 uint64_t;

#define MHD_PLATFORM_H
#include "../import-win/libmicrohttpd-0.9.17-w32/include/microhttpd.h"

#else

#include "microhttpd.h"

#endif

#include <map>

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef HTTPSERV_EXPORTS
#define HTTPSERVEXPORT DECLSPEC_EXPORT
#else
#define HTTPSERVEXPORT DECLSPEC_IMPORT
#endif

#define HTTPSERV_API(type)	EXTERN_C HTTPSERVEXPORT type __stdcall

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Hanlder Context
// -------------------------------------------------------------------------- //

class HTTPSERVEXPORT HandlerContext: public Object
{
	DECLARE_REF_CLASS(HandlerContext)

public:
	enum Method
	{
		HTTP_GET, HTTP_POST
	};

	typedef std::map<stringx, stringx> Values;
	typedef Values::value_type Value;

protected:
	MHD_Connection*	m_conn;
	Method	m_method;
	stringx	m_url;
	Values m_args;

public:
	int SendResponse(UINT statusCode, const void* message, UINT length, bool fConstance);

	inline int SendResponse(UINT statusCode, const std::string& value)
	{
		return SendResponse(statusCode, value.c_str(), value.length(), false);
	}
	inline int SendResponse(UINT statusCode, stringx value)
	{
		return SendResponse(statusCode, Encoding::get_UTF8()->FromUnicode(value));
	}
	inline int SendResponse(UINT statusCode, ref<StringBuilder> value)
	{
		return SendResponse(statusCode, value->ToString());
	}
	inline int SendResponse(UINT statusCode, ref<ByteArray> data, int offset, int length)
	{
		if (offset < 0 || length < 0 || offset + length > data->Count())
			cdec_throw(Exception(EC_OutOfRange));
		return SendResponse(statusCode, data->GetBuffer().ptr() + offset, length, false);
	}

	inline stringx GetUrl() { return m_url; }
	inline const Values& GetArgs() { return m_args; }
	stringx GetArg(stringx key);

	friend class Server;
};

interface IRequestHandler: public Object
{
	DECLARE_REF_CLASS(IRequestHandler)

	virtual int Handle(ref<HandlerContext> ctx) = 0;
};

interface IUrlDispatcher: public Object
{
	DECLARE_REF_CLASS(IUrlDispatcher)

	virtual ref<IRequestHandler> Dispatch(stringx url) = 0;
};

// -------------------------------------------------------------------------- //
// Default URL Dispatcher
// -------------------------------------------------------------------------- //

class HTTPSERVEXPORT DefaultUrlDispatcher: public IUrlDispatcher
{
	DECLARE_REF_CLASS(DefaultUrlDispatcher)

public:
	struct Item
	{
		stringx	Url;
		ref<IRequestHandler> Handler;
		Item(stringx url, ref<IRequestHandler> handler): Url(url), Handler(handler) {}
	};

protected:
	std::vector<Item> m_items;
	ref<IRequestHandler> m_defaultHandler;

public:
	DefaultUrlDispatcher(ref<IRequestHandler> defaultHandler): m_defaultHandler(defaultHandler) {}

	void Add(stringx url, ref<IRequestHandler> handler)
	{
		m_items.push_back(Item(url, handler));
	}

	ref<IRequestHandler> Dispatch(stringx url);
};

// -------------------------------------------------------------------------- //
// Server
// -------------------------------------------------------------------------- //

class HTTPSERVEXPORT Server: public Object
{
	DECLARE_REF_CLASS(Server)

protected:
	ref<IUrlDispatcher>		m_dispatcher;
	ref<Object>		m_globalContext;
	MHD_Daemon*		m_daemon;

public:
	Server(ref<IUrlDispatcher> dispatcher);
	void	Start(UINT port);

protected:
	static int ahc_echo(void* cls, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size,	void** ptr);
	static int parse_get_args(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
};

// -------------------------------------------------------------------------- //
// Static Handler
// -------------------------------------------------------------------------- //

class StaticHandler: public IRequestHandler
{
	DECLARE_REF_CLASS(StaticHandler);

	std::string	m_msg;

public:
	StaticHandler(PCSTR s, size_t l): m_msg(s, l) {}
	StaticHandler(PCSTR s): m_msg(s) {}
	StaticHandler(const std::string& s): m_msg(s) {}
	
	StaticHandler(stringx s): m_msg(Encoding::get_UTF8()->FromUnicode(s)) {}
	StaticHandler(PCWSTR s, size_t l): m_msg(Encoding::get_UTF8()->FromUnicode(s, l)) {}
	StaticHandler(PCWSTR s): m_msg(Encoding::get_UTF8()->FromUnicode(s, wstrlen16(s))) {}
	StaticHandler(const std::wstring16& s): m_msg(Encoding::get_UTF8()->FromUnicode(s)) {}

	int Handle(ref<HandlerContext> ctx)
	{
		return ctx->SendResponse(MHD_HTTP_OK, m_msg.c_str(), m_msg.size(), true);
	}
};

// -------------------------------------------------------------------------- //
CDEC_NS_END