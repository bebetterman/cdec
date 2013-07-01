#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Console: public Object
{
	DECLARE_REF_CLASS(Console)

public:
	static void WriteChar(WCHAR value);
	static void Write(stringx value);
	static inline void WriteLine() { WriteChar('\n'); }
	static inline void WriteLine(stringx value) { Write(value); WriteChar('\n'); }
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
