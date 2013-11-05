#include "stdafx.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// PreparedStatement
// -------------------------------------------------------------------------- //

void PreparedStatement::SetString(int index, stringx value)
{
	sql::SQLString value_s = Strx2SqlStr(value);
	m_impl->setString(index, value_s);
}

void PreparedStatement::SetInt(int index ,int value)
{
	m_impl->setInt(index,value);	
}

void PreparedStatement::SetUInt(int index, UINT value)
{
	m_impl->setUInt(index, value);
}

void PreparedStatement::SetInt64(int index ,INT64 value)
{
	m_impl->setInt64(index,value);
}

void PreparedStatement::SetUInt64(int index, UINT64 value)
{
	m_impl->setUInt64(index, value);
}

bool PreparedStatement::Execute()
{
	try
	{
		return m_impl->execute();
	}
	catch (sql::SQLException& e)
	{
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
	}
}

int PreparedStatement::ExecuteUpdate()
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

ref<ResultSet> PreparedStatement::ExecuteQuery()
{
	sql::ResultSet* rs = NULL;
    try
    {
        rs = m_impl->executeQuery();
	    return gc_new<ResultSet>(rs);
    }
    catch (sql::SQLException& e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

void PreparedStatement::Close()
{
	DESTORY_MYSQL_OBJECT(m_impl, PreparedStatement);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
