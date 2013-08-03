#pragma once

CDEC_NS_BEGIN

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

stringx SqlStr2Strx(sql::SQLString sqlStr, ref<Encoding> encode = Encoding::get_UTF8());

sql::SQLString Strx2SqlStr(stringx strx, ref<Encoding> encode = Encoding::get_UTF8());

// -------------------------------------------------------------------------- //
CDEC_NS_END
