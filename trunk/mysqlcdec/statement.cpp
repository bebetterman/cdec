#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

bool Statement::Execute(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql); 
#ifdef ENABLE_MYSQL_DEBUG
	printf("[MYSQL] Statement::Execute SQL: %s\n", sql_s.c_str());
#endif

	try
	{
		return m_impl->execute(sql_s);
	}
    catch (sql::SQLException& e)
    {
#ifdef ENABLE_MYSQL_DEBUG
		puts("MYSQL Exception");
		printf("Message: %s\n", e.what());
		printf("State: %s\n", e.getSQLState().c_str());
#endif
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

int Statement::ExecuteUpdate(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql);
#ifdef ENABLE_MYSQL_DEBUG
	printf("[MYSQL] Statement::ExecuteUpdate SQL: %s\n", sql_s.c_str());
#endif

	try
	{
		return m_impl->executeUpdate(sql_s);
	}
    catch (sql::SQLException& e)
    {
#ifdef ENABLE_MYSQL_DEBUG
		puts("MYSQL Exception");
		printf("Message: %s\n", e.what());
		printf("State: %s\n", e.getSQLState().c_str());
#endif
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

ref<ResultSet> Statement::ExecuteQuery(stringx sql)
{
	sql::SQLString sql_s = Strx2SqlStr(sql);
#ifdef ENABLE_MYSQL_DEBUG
	printf("[MYSQL] Statement::ExecuteQuery SQL: %s\n", sql_s.c_str());
#endif

	try
	{
		sql::ResultSet *rsimpl = m_impl->executeQuery(sql_s);
		return gc_new<ResultSet>(rsimpl);
	}
    catch (sql::SQLException& e)
    {
#ifdef ENABLE_MYSQL_DEBUG
		puts("MYSQL Exception");
		printf("Message: %s\n", e.what());
		printf("State: %s\n", e.getSQLState().c_str());
#endif
		cdec_throw(MysqlException(e.getErrorCode(), e.getSQLState(), e.what()));
    }
}

void Statement::Close()
{
	DESTORY_MYSQL_OBJECT(m_impl, Statement);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
