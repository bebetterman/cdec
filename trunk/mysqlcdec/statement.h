#pragma once

#include "StdAfx.h"

//#include "cppconn/statement.h"

//CDEC_NS_BEGIN
using namespace cdec;

class Connection;
//---------------------------------------------

//mysql driver class
class ResultSet;
class Statement : public Object
{
	DECLARE_REF_CLASS(Statement)
public:
	Statement();
	Statement(sql::Statement* stmt);
	~Statement();

public:


    bool execute(stringx sql);
    int executeUpdate(stringx sql);
    ref<ResultSet> executeQuery(stringx sql);


protected:
	sql::Statement *m_impl;
};



//---------------------------------------------

//CDEC_NS_END
