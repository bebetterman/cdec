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
	if (m_impl != NULL && !m_impl->isClosed())
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
int ResultSet::RowsCount() 
{
	return m_impl->rowsCount();
}


stringx ResultSet::GetString(stringx columLabel) const
{
	sql::SQLString sqlStr = m_impl->getString(Strx2SqlStr(columLabel));
	return SqlStr2Strx(sqlStr);
}

int ResultSet::GetInt(UINT32 columIndex) const
{

	int sqlInt = m_impl->getInt(columIndex);

	//convert sql::SQLString to stringx
	return sqlInt;
}

int ResultSet::GetInt(stringx columLabel) const
{

	int sqlInt = m_impl->getInt(Strx2SqlStr(columLabel));

	return sqlInt;
}

UINT ResultSet::GetUInt(int column)
{
	return m_impl->getUInt(column);
}

INT64 ResultSet::GetInt64(stringx columLabel) const
{

	INT64 sqlInt = m_impl->getInt64(Strx2SqlStr(columLabel));

	return sqlInt;
}
//---------------------------------------------
//CDEC_NS_BEGIN
