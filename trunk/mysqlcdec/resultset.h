#pragma once

#include "StdAfx.h"

//CDEC_NS_BEGIN

//-----------------------------------------------------
class ResultSet : public Object
{
public:
	//---------constructor and deconstructor-----------
	ResultSet();
	ResultSet(sql::ResultSet *res);
	~ResultSet();
	
	//---------db operator-----------------------------
	bool Next();
	stringx getString(UINT32 columIndex) const;
	stringx getString(stringx columLabel) const;
	UINT64 RowsCount();

protected:
	//ref<Encoding>	 encode;
	sql::ResultSet *m_impl;
};


//-----------------------------------------------------


//CDEC_NS_END