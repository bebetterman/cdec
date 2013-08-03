#include "stdafx.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// PrepareStatement
// -------------------------------------------------------------------------- //

void PrepareStatement::SetString(int index, stringx value)
{
	sql::SQLString value_s = Strx2SqlStr(value);
	m_impl->setString(index, value_s);
}

void PrepareStatement::SetInt(int index ,int value)
{
	m_impl->setInt(index,value);
	
}

void PrepareStatement::SetInt64(int index ,INT64 value)
{
	m_impl->setInt64(index,value);
}

int PrepareStatement::ExecuteUpdate()
{
    try
    {
        return m_impl->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

ref<ResultSet> PrepareStatement::ExecuteQuery()
{
	sql::ResultSet* rs = NULL;
    try
    {
        rs = m_impl->executeQuery();
    }
    catch (sql::SQLException& e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
    return gc_new<ResultSet>(rs);
}

void PrepareStatement::Close()
{
	DESTORY_MYSQL_OBJECT(m_impl, PrepareStatement);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
