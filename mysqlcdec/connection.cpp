#include "stdafx.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Connection
// -------------------------------------------------------------------------- //

Connection::Connection(ref<ConnectionManager> manager, int index, sql::Connection* impl)
{
	m_manager = manager;
	m_index = index;
	m_impl = impl;
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
#ifdef _DEBUG
		puts("MYSQL Exception");
		printf("Message: %s\n", e.what());
		printf("State: %s\n", e.getSQLState().c_str());
#endif
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

void Connection::Return()
{
	if (m_impl != NULL)
	{
		m_manager->ReturnByAgent(m_index);
		m_manager = NULL;
		m_impl = NULL;
		m_index = -1;
	}
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
