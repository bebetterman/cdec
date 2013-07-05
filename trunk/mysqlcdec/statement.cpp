#include "stdafx.h"
#include "dbUtil.h"
#include "statement.h"


//CDEC_NS_BEGIN

//-------------------------------------------------

Statement::Statement()
{
	m_impl = NULL;
}

Statement::Statement(sql::Statement* stmt)
	:m_impl(stmt)
{
	
}

Statement::~Statement()
{
	if (m_impl != NULL)
	{
		m_impl->close();
		m_impl = NULL;
	}
}

bool Statement::execute(stringx sql)
{
	//convert stringx to SQLString
	sql::SQLString sqlStr = Strx2SqlStr(sql); 
	return m_impl->execute(sqlStr);
}

int Statement::executeUpdate(stringx sql)
{
	//convert stringx to SQLString
	sql::SQLString sqlStr = Strx2SqlStr(sql); 
	return m_impl->executeUpdate(sqlStr);
}



ref<ResultSet> Statement::executeQuery(stringx sql)
{
	//convert stringx to SQLString
	sql::SQLString sqlStr = Strx2SqlStr(sql);
	sql::ResultSet *res = m_impl->executeQuery(sqlStr);
	ref<ResultSet> gc_res = gc_new<ResultSet>(res);
	
	return gc_res;
}

//-------------------------------------------------

//CDEC_NS_END