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
			puts("MYSQL Exception");
			printf("Error NO: %d\n",_sqlerr);
			printf("Message: %s\n",_sqlstat.c_str());
			printf("State: %s\n", _reason.c_str());
		}
	};
}

// -------------------------------------------------------------------------- //
// Resource Pool
// -------------------------------------------------------------------------- //

namespace cdec
{
	interface IResource: public Object
	{
		DECLARE_REF_CLASS(IResource)

		virtual int Index() = 0;
		virtual void Dispose() = 0;
		virtual bool IsDamage() = 0;
		virtual void SetDamage(bool isDamage) = 0;
	};

	interface IResourceFactory: public Object
	{
		DECLARE_REF_CLASS(IResourceFactory)

		virtual ref<IResource> Make(int index) = 0;
		virtual void Dispose() = 0;
	};

	interface IResourcePool: public Object
	{
		DECLARE_REF_CLASS(IResourcePool)

		virtual ref<IResource>	Take() = 0;
		virtual ref<IResource>	TakeCreate() = 0;
		virtual void Return(ref<IResource>) = 0;

		virtual void Dispose() = 0;
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
