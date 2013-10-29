#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

void DateTime::Set(int year, int month, int day, int hour, int minute, int second, int milliseconds)
{
#ifdef X_OS_WINDOWS
	m_st.wYear = year;
	m_st.wMonth = month;
	m_st.wDay = day;
	m_st.wHour = hour;
	m_st.wMinute = minute;
	m_st.wSecond = second;
	m_st.wMilliseconds = milliseconds;

	// todo: set wDayOfWeek
#else
	m_tm.tm_year = year - 1900;
	m_tm.tm_mon = month - 1;
	m_tm.tm_mday = day;
	m_tm.tm_hour = hour;
	m_tm.tm_min = minute;
	m_tm.tm_sec = second;
	m_milli = milliseconds;
	m_tm.tm_gmtoff = 0;	// UTC

	// todo: set wDayOfWeek
#endif
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
	m_tm = *localtime((time_t*)&timestamp);
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
	m_tm = *gmtime((time_t*)&timestamp);
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
	return mktime(&m_tm) + 3600 * 8;
#endif
}

DateTime DateTime::Now()
{
#ifdef X_OS_WINDOWS
	SYSTEMTIME st;
	GetLocalTime(&st);
	DateTime dt;
	dt.SetSystemTime(st);
	return dt;
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
	DateTime dt;
	dt.SetSystemTime(st);
	return dt;
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

stringx DateTime::Format()
{
	ref<StringBuilder> sb = gc_new<StringBuilder>();
	int year = GetYear(), month = GetMonth();
#ifndef X_OS_WINDOWS
	year += 1900;
	month += 1;
#endif
	sb->Append(Converter::ToString(year));
	sb->Append('-');
	stringx format = Converter::ToString(month);
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append('-');
	format = Converter::ToString(this->GetDay());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append(' ');
	format = Converter::ToString(this->GetHour());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append(':');
	format = Converter::ToString(this->GetMinute());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	sb->Append(':');
	format = Converter::ToString(this->GetSecond());
	sb->Append(format.Length() == 2 ? format : __X("0") + format);
	return sb->ToString();
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
