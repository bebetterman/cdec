#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Math: public Object
{
	DECLARE_REF_CLASS(Math)

public:
	template<typename T>
	static T Min(T a, T b) { return a <= b ? a : b; }

	template<typename T>
	static T Max(T a, T b) { return a >= b ? a : b; }
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
