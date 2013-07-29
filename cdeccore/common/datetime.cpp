#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

CDECCOREAPI(stringx) DateTime_Format(const DateTime& dt)
{
	ref<StringBuilder> sb = gc_new<StringBuilder>();
	sb->Append(Converter::ToString(dt.GetYear()));
	sb->Append('-');
	sb->Append(Converter::ToString(dt.GetMonth()));
	sb->Append('-');
	sb->Append(Converter::ToString(dt.GetDay()));
	sb->Append(' ');
	sb->Append(Converter::ToString(dt.GetHour()));
	sb->Append(':');
	sb->Append(Converter::ToString(dt.GetMinute()));
	sb->Append(':');
	sb->Append(Converter::ToString(dt.GetSecond()));
	return sb->ToString();
}

// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

#else

#endif
// -------------------------------------------------------------------------- //
CDEC_NS_END
