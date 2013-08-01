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

int ResultSet::RowsCount() 
{
	return m_impl->rowsCount();
}

int ResultSet::GetInt(int index) { return m_impl->getInt(index); }

int ResultSet::GetInt(stringx key)
{
	sql::SQLString skey = Strx2SqlStr(key);
	return m_impl->getInt(skey);
}

INT64 ResultSet::GetInt64(int index) { return m_impl->getInt64(index); }

INT64 ResultSet::GetInt64(stringx key)
{
	sql::SQLString skey = Strx2SqlStr(key);
	return m_impl->getInt64(skey);
}

stringx ResultSet::GetString(int index)
{
	sql::SQLString sqlStr = m_impl->getString(index);
	return SqlStr2Strx(sqlStr);
}

stringx ResultSet::GetString(stringx key)
{
	sql::SQLString skey = Strx2SqlStr(key);
	sql::SQLString sqlStr = m_impl->getString(skey);
	return SqlStr2Strx(sqlStr);
}

//---------------------------------------------
//CDEC_NS_BEGIN
