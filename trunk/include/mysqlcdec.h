

#include <cdeccore.h>

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


#include  "mysqlcdec/statement.h"
#include  "mysqlcdec/resultset.h"
#include  "mysqlcdec/prepareStatement.h"
#include  "mysqlcdec/connection.h"
#include  "mysqlcdec/dbUtil.h"



