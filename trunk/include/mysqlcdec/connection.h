#pragma once
#include "stdafx.h"
using namespace  cdec;

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //
#ifdef CONNECTION_EXPORTS
#define CONNECTIONEXPORT DECLSPEC_EXPORT
#else
#define CONNECTIONEXPORT DECLSPEC_IMPORT
#endif

#define CONNECTION_API(type)	EXTERN_C CONNECTIONEXPORT type __stdcall

class  CONNECTIONEXPORT Connection : public Object
{
	DECLARE_REF_CLASS(Connection)
private:
	sql::Driver * m_driver;
	sql::Connection *  m_impl;
public:
	Connection();
	Connection(stringx dburl,stringx uname,stringx pwd,stringx database);
	Connection(stringx dburl,stringx uname,stringx pwd);
	~Connection();
	ref<Statement> CreateStatement();
	ref<PrepareStatement> CreatePrepareStatement(stringx sql);
	void Close();
	friend class DbUtil;
	friend class DbConnection;
	friend class DbConnectionManager;
};