#pragma once

#include "stdafx.h"

//CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Used in this project only 
// -------------------------------------------------------------------------- //
#ifdef RESULTSET_EXPORTS
#define RESULTSETEXPORT DECLSPEC_EXPORT
#else
#define RESULTSETEXPORT DECLSPEC_IMPORT
#endif

#define RESULTSET_API(type)	EXTERN_C RESULTSETEXPORT type __stdcall

//-----------------------------------------------------
class  RESULTSETEXPORT ResultSet : public Object
{
	DECLARE_REF_CLASS(ResultSet)
public:
	//---------constructor and deconstructor-----------
	ResultSet();
	ResultSet(sql::ResultSet *res);
	~ResultSet();
	
	//---------db operator-----------------------------
	bool Next();
	int RowsCount();

	int		GetInt(int index);
	int		GetInt(stringx key);

	INT64	GetInt64(int index);
	INT64	GetInt64(stringx key);

	stringx	GetString(int index);
	stringx	GetString(stringx key);

protected:
	//ref<Encoding>	 encode;
	sql::ResultSet *m_impl;
};

//-----------------------------------------------------


//CDEC_NS_END