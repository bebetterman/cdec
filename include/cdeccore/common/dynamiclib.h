#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Dynamic Library
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT DynamicLibrary: public Object
{
	DECLARE_REF_CLASS(DynamicLibrary)

protected:
	void*	m_hModule;

public:
	DynamicLibrary(): m_hModule(NULL) {}

	bool	Load(stringx moduleName);
	void*	GetAddress(stringx exportName);
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
