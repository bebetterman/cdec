#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

bool Statement::Execute(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql); 
	return m_impl->execute(sql_s);
}

int Statement::ExecuteUpdate(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql);
	return m_impl->executeUpdate(sql_s);
}

ref<ResultSet> Statement::ExecuteQuery(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql);
	sql::ResultSet *rsimpl = m_impl->executeQuery(sql_s);
	return gc_new<ResultSet>(rsimpl);
}

void Statement::Close()
{
	DESTORY_MYSQL_OBJECT(m_impl, Statement);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
