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


    bool Execute(stringx sql);
    int ExecuteUpdate(stringx sql);
    ref<ResultSet> ExecuteQuery(stringx sql);


protected:
	sql::Statement *m_impl;
};



//---------------------------------------------

//CDEC_NS_END
