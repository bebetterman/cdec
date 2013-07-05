#pragma once
#include "stdafx.h"

using namespace  cdec;

//CDEC_NS_BEGIN


//--------------------------------------------
/*convert sql::SQLString to cdec::stringx*/
stringx SqlStr2Strx(sql::SQLString sqlStr, ref<Encoding>	encode = Encoding::get_UTF8());

/*convert cdec::stringx to sql::SQLString*/
sql::SQLString Strx2SqlStr(stringx strx, ref<Encoding> encode = Encoding::get_UTF8());


class DbUtil: public Object
{
	DECLARE_REF_CLASS(Dbconn)

protected:
	stringx m_dburl;
	stringx m_uname;
	stringx m_pwd;
	stringx m_database;

	stringx m_desc;

	ref<Connection> m_conn;
	//sql::Driver * m_driver;
	//sql::Statement * m_stmt;
	//sql::ResultSet * m_res;

public:
	DbUtil(stringx pathConfig);
	ref<Connection> Conn();
	ref<Connection> Getconn(){return m_conn;};
	bool Execute(stringx sql);
	int ExecuteUpdate(stringx sql);
	//ref<ResultSet> ExecuteQuery(stringx sql);
	void CloseConn();
};



// -------------------------------------------------------------------------- //
//CDEC_NS_END