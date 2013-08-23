#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

CDECCOREAPI(stringx) DateTime_Format(const DateTime& dt)
{
	ref<StringBuilder> sb = gc_new<StringBuilder>();
	sb->Append(Converter::ToString(dt.GetYear()));
	sb->Append('-');
	stringx format = Converter::ToString(dt.GetMonth());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append('-');
	format = Converter::ToString(dt.GetDay());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append(' ');
	format = Converter::ToString(dt.GetHour());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append(':');
	format = Converter::ToString(dt.GetMinute());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append(':');
	format = Converter::ToString(dt.GetSecond());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	return sb->ToString();
}

// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

#else

#endif
// -------------------------------------------------------------------------- //
CDEC_NS_END
