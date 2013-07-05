#pragma once
#include "stdafx.h"

using namespace  cdec;
class PrepareStatement: public Object
{
	_CDEC_SHARED_CLASS(PrepareStatement)
private:
	 sql::PreparedStatement *  m_impl;
public:
	PrepareStatement(sql::PreparedStatement * pstmt):m_impl(pstmt){};
	PrepareStatement(stringx sql);
	void Close();
	int ExecuteUpdate();
	void SetString(UINT index, stringx value);
	void SetInt(UINT index ,INT value);
	//ref<ResultSet> ExecuteQuery(stringx sql);
};
