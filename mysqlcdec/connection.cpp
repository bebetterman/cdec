#include "stdafx.h"
#include "pool.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Connection
// -------------------------------------------------------------------------- //

Connection::Connection(ref<ConnectionManager> manager, ref<IResource> rc)
{
	m_manager = manager;
	m_rc = rc;
	m_impl = (ref_cast<ConnResource>(rc))->Impl();
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
		m_rc->SetDamage(true);
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

ref<PreparedStatement> Connection::CreatePreparedStatement(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql);
#ifdef ENABLE_MYSQL_DEBUG
	printf("[MYSQL] CreatePreparedStatement SQL: %s\n", sql_s.c_str());
#endif
    try
    {
		sql::PreparedStatement* sprstmt = m_impl->prepareStatement(sql_s);
		return gc_new<PreparedStatement>(sprstmt);
    }
    catch (sql::SQLException& e)
    {
#ifdef ENABLE_MYSQL_DEBUG
		puts("MYSQL Exception");
		printf("Message: %s\n", e.what());
		printf("State: %s\n", e.getSQLState().c_str());
#endif
		m_rc->SetDamage(true);
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
		m_manager->ReturnByAgent(m_rc);
		m_manager = NULL;
		m_rc = NULL;
		m_impl = NULL;
	}
}


void Connection::Commit()
{
	if (m_impl != NULL)
	{
		try
		{
			m_impl->commit();
		}
		catch (sql::SQLException &e)
		{
			cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
		}
	}
}
void Connection::SetAutoCommit(bool flag)
{ 
	if (m_impl != NULL)
	{
		try
		{
			m_impl->setAutoCommit(flag);
		}
		catch (sql::SQLException &e)
		{
			cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
		}
	}
}

void Connection::Rollback()
{ 
	if (m_impl != NULL)
	{
		try
		{
			m_impl->rollback();
		}
		catch (sql::SQLException &e)
		{
			cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
		}
	}
}


// -------------------------------------------------------------------------- //
CDEC_NS_END
