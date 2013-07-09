#pragma once
#include "stdafx.h"
using namespace  cdec;


// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //
#ifdef PREPARESTATEMENT_EXPORTS
#define PREPARESTATEMENTEXPORT DECLSPEC_EXPORT
#else
#define PREPARESTATEMENTEXPORT DECLSPEC_IMPORT
#endif

#define PREPARESTATEMENT_API(type)	EXTERN_C PREPARESTATEMENTEXPORT type __stdcall

class  PREPARESTATEMENTEXPORT PrepareStatement: public Object
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
