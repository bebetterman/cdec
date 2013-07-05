#pragma once
#include "stdafx.h"
using namespace  cdec;

class Connection : public Object
{
	_CDEC_SHARED_CLASS(Connection)
private:
	sql::Driver * m_driver;
	sql::Connection *  m_impl;
public:
	Connection(stringx dburl,stringx uname,stringx pwd,stringx database);
	Connection(stringx dburl,stringx uname,stringx pwd);
	ref<Statement> CreateStatement();
	ref<PrepareStatement> prepareStatement(stringx sql);
	void Close();
	friend class DbUtil;
};