#include "stdafx.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Connection
// -------------------------------------------------------------------------- //

Connection::Connection()
{
	m_driver = get_driver_instance();
	m_impl = NULL;
}

void Connection::Connect(stringx url, stringx user, stringx pass)
{
	sql::SQLString url_s = Strx2SqlStr(url);
	sql::SQLString user_s = Strx2SqlStr(user);
	sql::SQLString pass_s = Strx2SqlStr(pass);

	Close();
	m_impl = m_driver->connect(url_s, user_s, pass_s);
}

void Connection::SetSchema(stringx name)
{
	ASSERT(m_impl != NULL);
	sql::SQLString name_s = Strx2SqlStr(name);
	m_impl->setSchema(name_s);
}

ref<Statement> Connection::CreateStatement()
{
    try
    {
		sql::Statement* stmt = m_impl->createStatement();
		return gc_new<Statement>(stmt);
    }
    catch (sql::SQLException& e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

ref<PrepareStatement> Connection::CreatePrepareStatement(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql);
    try
    {
		sql::PreparedStatement* sprstmt = m_impl->prepareStatement(sql_s);
		return gc_new<PrepareStatement>(sprstmt);
    }
    catch (sql::SQLException& e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

// -------------------------------------------------------------------------- //
// Easy operations
// -------------------------------------------------------------------------- //

bool Connection::Execute(stringx sql)
{
	try
	{
		ref<Statement> stmt = CreateStatement();
		return stmt->Execute(sql);
	}
    catch (sql::SQLException& e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

int Connection::ExecuteUpdate(stringx sql)
{
    try
    {
		ref<Statement> stmt = CreateStatement();
		return stmt->ExecuteUpdate(sql);
    }
    catch (sql::SQLException &e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

ref<ResultSet> Connection::ExecuteQuery(stringx sql)
{
    try
    {
		ref<Statement> stmt = CreateStatement();
		return stmt->ExecuteQuery(sql);
    }
    catch (sql::SQLException &e)
    {
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
	}
}

void Connection::Close()
{
	DESTORY_MYSQL_OBJECT(m_impl, Connection);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
