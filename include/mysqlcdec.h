#pragma once

#ifndef __CDECCORE__
#error cdeccore Expected
#endif

#ifndef __CDEC_MYSQL__
#define __CDEC_MYSQL__
#endif

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //

#ifdef MYSQLCDEC_EXPORTS
#define MYSQLCDECEXPORT		DECLSPEC_EXPORT
#else
#define MYSQLCDECEXPORT		DECLSPEC_IMPORT
#endif

// -------------------------------------------------------------------------- //
// Predeclaration of mysqlconn internal classes
// -------------------------------------------------------------------------- //

namespace sql
{
	class Statement;
	class ResultSet;
	class PreparedStatement;
	class Connection;
	class Driver;
}

// -------------------------------------------------------------------------- //
// Exception
//
//   Note that MYSQL error code is not mapped into Cdec exception code, the latter
// is always EC_FAIL. You may refer SQLErr field for MYSQL error code.
//
// -------------------------------------------------------------------------- //

namespace cdec
{
	struct MysqlException : Exception
	{
		const int SQLErr;
		const std::string SQLState;
		const std::string Reason;

		MysqlException(int _sqlerr, const std::string& _sqlstat, const std::string& _reason):
		Exception(EC_Fail), SQLErr(_sqlerr), SQLState(_sqlstat), Reason(_reason)
		{
		}
	};
}

// -------------------------------------------------------------------------- //
// Following headers
// -------------------------------------------------------------------------- //

#include "mysqlcdec/resultset.h"
#include "mysqlcdec/statement.h"
#include "mysqlcdec/prepareStatement.h"
#include "mysqlcdec/connection.h"
#include "mysqlcdec/dbUtil.h"

// -------------------------------------------------------------------------- //
