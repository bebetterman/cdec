#pragma once

// -------------------------------------------------------------------------- //
// mysqlconn headers
// -------------------------------------------------------------------------- //

#if defined(X_OS_WINDOWS)
	#include "../import-win/mysqlconn-1.1.3/include/fake_boost.h"
	#include "../import-win/mysqlconn-1.1.3/include/mysql_connection.h"
	#include "../import-win/mysqlconn-1.1.3/include/mysql_driver.h"
	#include "../import-win/mysqlconn-1.1.3/include/cppconn/resultset.h"
	#include "../import-win/mysqlconn-1.1.3/include/cppconn/exception.h"
	#include "../import-win/mysqlconn-1.1.3/include/cppconn/statement.h"
	#include "../import-win/mysqlconn-1.1.3/include/cppconn/prepared_statement.h"
#else
	#include <mysql_connection.h>
	#include <cppconn/driver.h>
	#include <cppconn/exception.h>
	#include <cppconn/resultset.h>
	#include <cppconn/statement.h>
	#include <cppconn/prepared_statement.h>
#endif

// -------------------------------------------------------------------------- //
// Convertion between sql::SQLString and stringx
// -------------------------------------------------------------------------- //

extern ref<Encoding> g_Utf8;

inline stringx SqlStr2Strx(sql::SQLString sqlStr)
{
	return g_Utf8->ToUnicode(sqlStr.asStdString());
}

inline sql::SQLString Strx2SqlStr(stringx strx)
{
    std::string stdSql = g_Utf8->FromUnicode(strx);
    return sql::SQLString(stdSql);
}

template<class T>
inline void DestroyMysqlObject(T*& ptr, PCSTR message)
{
	if (ptr != NULL)
	{
		try
		{
			ptr->close();
			delete ptr;
		}
		catch (sql::SQLException& e)
		{
			// Ignore the exception
			puts(message);
			puts(e.what());
		}
		ptr = NULL;
	}
}

#define DESTORY_MYSQL_OBJECT(ptr, type)			\
	DestroyMysqlObject(ptr, "Exception raised when destroying " #type " object");

// -------------------------------------------------------------------------- //
