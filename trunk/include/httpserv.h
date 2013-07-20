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

// Set the DEBUG_HTTPCDEC macro to enable debugging
#define DEBUG_HTTPCDEC

#ifdef DEBUG_HTTPCDEC

#define HTTPCDEC_DEBUG_ONLY(x)	(x)

inline void HttpCdecDebugLog(PCSTR s) { puts(s); }

inline void HttpCdecDebugFormat(PCSTR fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

#else

#define HTTPCDEC_DEBUG_ONLY(x)	(0)

inline void HttpCdecDebugLog(PCSTR) {}

inline void HttpCdecDebugFormat(PCSTR fmt, ...) {}

#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Hanlder Context
// -------------------------------------------------------------------------- //

interface IRequestHandler;

class HTTPSERVEXPORT HandlerContext: public Object
{
	DECLARE_REF_CLASS(HandlerContext)

public:
	enum Method
	{
		HTTP_NONE, HTTP_GET, HTTP_POST
	};

	typedef std::map<stringx, stringx> StringPairMap;
	typedef StringPairMap::value_type Value;

	typedef std::map<stringx, std::string>	PostMap;
	typedef std::vector<BYTE>				PostRaw;

protected:
	ref<IRequestHandler>	m_handler;

	MHD_Connection*		m_conn;
	Method				m_method;
	stringx				m_url;

	MHD_PostProcessor*	m_postprocessor;

	bool			m_keepPostData;

	StringPairMap	m_getArgs;
	PostMap			m_postArgs;
	PostRaw			m_postRaw;

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
	inline Method GetMethod() { return m_method; }
	inline const StringPairMap& GetArgs() { return m_getArgs; }
	inline const PostMap& PostArgs() { return m_postArgs; }
	inline ref<ByteArray> PostData() { return gc_new<ByteArray>(&m_postRaw[0], m_postRaw.size()); }

	void Dispose();

	friend class Server;
};

interface IRequestHandler: public Object
{
	DECLARE_REF_CLASS(IRequestHandler)

	virtual int Handle(ref<HandlerContext> ctx) = 0;

	// Request handler config
	virtual bool KeepPostData() { return false; }
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

protected:	// Callback functions
	static int OnRequestHandler(void* cls, MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size,	void** ptr);
	static int OnParseGetArgs(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
	static int OnPostDataIterator(void *cls, enum MHD_ValueKind kind, const char *key, const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size);
	static void OnRequestCompleted(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe);
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
