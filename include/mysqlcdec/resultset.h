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
	stringx GetString(UINT32 columIndex) const;
	stringx GetString(stringx columLabel) const;
	int GetInt(UINT32 columIndex) const;
	int GetInt(stringx columLabel) const;

	UINT	GetUInt(int column);

	INT64 GetInt64(stringx columLabel) const;
	int RowsCount();

protected:
	//ref<Encoding>	 encode;
	sql::ResultSet *m_impl;
};


//-----------------------------------------------------


//CDEC_NS_END