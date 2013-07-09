#pragma once
#include "stdafx.h"
using namespace  cdec;


class PrepareStatement: public Object
{
	DECLARE_REF_CLASS(PrepareStatement)
private:
	 sql::PreparedStatement *  m_impl;
public:
	PrepareStatement(sql::PreparedStatement * pstmt):m_impl(pstmt){};
	PrepareStatement(stringx sql);
	void Close();
	int ExecuteUpdate();
	ref<ResultSet> ExecuteQuery();
	void SetString(UINT index, stringx value);
	void SetInt(UINT index, int value);
};
