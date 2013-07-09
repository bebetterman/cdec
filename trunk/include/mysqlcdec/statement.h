#pragma once

#include "stdafx.h"


//CDEC_NS_BEGIN
using namespace cdec;


// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //
#ifdef STATEMENT_EXPORTS
#define STATEMENTEXPORT DECLSPEC_EXPORT
#else
#define STATEMENTEXPORT DECLSPEC_IMPORT
#endif

#define STATEMENT_API(type)	EXTERN_C STATEMENTEXPORT type __stdcall

class Connection;
//---------------------------------------------

//mysql driver class
class ResultSet;
class  STATEMENTEXPORT Statement : public Object
{
	DECLARE_REF_CLASS(Statement)
public:
	Statement();
	Statement(sql::Statement* stmt);
	~Statement();

public:


    bool Execute(stringx sql);
    int ExecuteUpdate(stringx sql);
    ref<ResultSet> ExecuteQuery(stringx sql);


protected:
	sql::Statement *m_impl;
};



//---------------------------------------------

//CDEC_NS_END
