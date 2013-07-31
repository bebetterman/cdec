#pragma once
#include "stdafx.h"
using namespace  cdec;

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //
#ifdef DBUTIL_EXPORTS
#define DBUTILEXPORT DECLSPEC_EXPORT
#else
#define DBUTILEXPORT DECLSPEC_IMPORT
#endif

#define DBUTIL_API(type)	DBUTILEXPORT type __stdcall

//--------------------------------------------
/*convert sql::SQLString to cdec::stringx*/
stringx SqlStr2Strx(sql::SQLString sqlStr, ref<Encoding> encode = Encoding::get_UTF8());
/*convert cdec::stringx to sql::SQLString*/
sql::SQLString Strx2SqlStr(stringx strx, ref<Encoding> encode = Encoding::get_UTF8());

// -------------------------------------------------------------------------- //
// DbConnectionManager
// -------------------------------------------------------------------------- //

class DBUTILEXPORT DbConnection: public Object
{
	DECLARE_REF_CLASS(DbConnection)

protected:
	ref<Connection> m_conn;

public:
	DbConnection(ref<Connection> conn): m_conn(conn) {}

	ref<Connection>	Conn() { return m_conn; }

	bool	Execute(stringx sql);
	int		ExecuteUpdate(stringx sql);
	ref<ResultSet>	ExecuteQuery(stringx sql);
	
	// Note: Do NOT call this in your codes, use DbConnectionManager::Return instead
	void	Dispose();
};

// -------------------------------------------------------------------------- //
// DbConnectionManager
// -------------------------------------------------------------------------- //

struct DBUTILEXPORT DbConfig
{
	stringx	Url;
	stringx	Username, Password;
	stringx Database;

	void LoadConfigXml(stringx path);
};

class DBUTILEXPORT DbConnectionManager: public Object
{
	DECLARE_REF_CLASS(DbConnectionManager)

protected:
	DbConfig	m_dbconfig;

public:
	DbConnectionManager(DbConfig dbconfig);

	ref<DbConnection> Take();
	void Return(ref<DbConnection> dbcon) { dbcon->Dispose(); }

	void	Dispose() {}
};

// -------------------------------------------------------------------------- //

#if 0

class DBUTILEXPORT DbUtil: public Object
{
	DECLARE_REF_CLASS(DbUtil)
protected:
	stringx m_dburl;
	stringx m_uname;
	stringx m_pwd;
	stringx m_database;
	stringx m_desc;

	ref<Connection> m_conn;

public:
	DbUtil(stringx pathConfig);
	ref<Connection> Conn();
	ref<Connection> GetConn(){return m_conn;};
	bool Execute(stringx sql);
	int ExecuteUpdate(stringx sql);
	ref<ResultSet> ExecuteQuery(stringx sql);
	void CloseConn();
};

#endif

// -------------------------------------------------------------------------- //