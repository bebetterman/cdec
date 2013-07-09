#pragma once
#include "stdafx.h"


//CDEC_NS_BEGIN
//---------------------------------------------
ResultSet::ResultSet()
{
	m_impl = NULL;
}

ResultSet::ResultSet(sql::ResultSet *res):m_impl(res)
{
	
}

ResultSet::~ResultSet()
{
	if (m_impl != NULL && m_impl->isClosed())
	{
		m_impl->close();
		m_impl = NULL;
	}
}

bool ResultSet::Next()
{
	return m_impl->next();
}

stringx ResultSet::GetString(UINT32 columIndex) const
{

	sql::SQLString sqlStr = m_impl->getString(columIndex);

	//convert sql::SQLString to stringx
	return SqlStr2Strx(sqlStr);
}
UINT64 ResultSet::RowsCount() 
{
	return m_impl->rowsCount();
}


stringx ResultSet::GetString(stringx columLabel) const
{
	sql::SQLString sqlStr = m_impl->getString(Strx2SqlStr(columLabel));
	return SqlStr2Strx(sqlStr);
}

//---------------------------------------------
//CDEC_NS_BEGIN
