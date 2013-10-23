#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void DateTime::Set(int year, int month, int day, int hour, int minute, int second, int milliseconds)
{
	DateTime dt;

#ifdef X_OS_WINDOWS
	dt.m_st.wYear = year;
	dt.m_st.wMonth = month;
	dt.m_st.wDay = day;
	dt.m_st.wHour = hour;
	dt.m_st.wMinute = minute;
	dt.m_st.wSecond = second;
	dt.m_st.wMilliseconds = milliseconds;

	// todo: set wDayOfWeek
#else
	dt.m_tm.tm_year = year;
	dt.m_tm.tm_mon = month;
	dt.m_tm.tm_mday = day;
	dt.m_tm.tm_hour = hour;
	dt.m_tm.tm_min = minute;
	dt.m_tm.tm_sec = second;
	dt.m_milli = milliseconds;

	// todo: set wDayOfWeek
#endif

	return dt;
}

void DateTime::SetLocal(INT64 timestamp, int milli)
{
#ifdef X_OS_WINDOWS
	FILETIME ft, ftl;
	(INT64&)ft = timestamp * 10000000 + 116444736000000000;

	if (!FileTimeToLocalFileTime(&ft, &ftl))
		cdec_throw_win32_lasterr(Exception);

    SYSTEMTIME pst;
	if (!FileTimeToSystemTime(&ftl, &m_st))
		cdec_throw_win32_lasterr(Exception);
#else
	m_tm = *localtime(&timestamp);
	m_milli = milli;
#endif
}

void DateTime::SetUtc(INT64 timestamp, int milli)
{
#ifdef X_OS_WINDOWS
	FILETIME ft;
	(INT64&)ft = timestamp * 10000000 + 116444736000000000; 

    SYSTEMTIME pst;
	if (!FileTimeToSystemTime(&ft, &m_st))
		cdec_throw_win32_lasterr(Exception);
#else
	m_tm = *gmtime(&timestamp);
	m_milli = milli;
#endif
}

INT64 DateTime::ToTimestamp()
{
#ifdef X_OS_WINDOWS
	FILETIME ft;
    if (!SystemTimeToFileTime(&m_st, &ft))
		cdec_throw_win32_lasterr(Exception);
 
	INT64 nll = (INT64&)ft;
	return (nll - 116444736000000000) / 10000000;
#else
	cdec_throw(Exception(EC_NotImplemented));
#endif
}

DateTime DateTime::Now()
{
#ifdef X_OS_WINDOWS
	SYSTEMTIME st;
	GetLocalTime(&st);
	return FromSystemTime(st);
#else
	timespec tmspec;
	clock_gettime(CLOCK_REALTIME, &tmspec);
	DateTime dt;
	dt.SetTimeSpecLocal(tmspec);
	return dt;
#endif
}

DateTime DateTime::NowUtc()
{
#ifdef X_OS_WINDOWS
	SYSTEMTIME st;
	GetSystemTime(&st);
	return FromSystemTime(st);
#else
	timespec tmspec;
	clock_gettime(CLOCK_REALTIME, &tmspec);
	DateTime dt;
	dt.SetTimeSpecUtc(tmspec);
	return dt;
#endif
}

TimeValue DateTime::NowTimeValue()
{
#ifdef X_OS_WINDOWS
	TimeValue tv;
	__timeb64 tb;
	_ftime64(&tb);
	tv.Time = tb.time;
	tv.Micro = tb.millitm * 1000;
	tv.TimeZone = tb.timezone;
	tv.DstFlag = tb.dstflag;
	return tv;
#else
	TimeValue val;
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	val.Time = tv.tv_sec;
	val.Micro = tv.tv_usec;
	val.TimeZone = tz.tz_minuteswest;
	val.DstFlag = tz.tz_dsttime;
	return val;
#endif
}

stringx DateTime_Format(const DateTime& dt)
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
CDEC_NS_END
