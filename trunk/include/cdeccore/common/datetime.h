#pragma once

#ifdef X_OS_WINDOWS
#	include <sys/timeb.h>
#else
#	include <sys/time.h>
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Predeclaration, do not use
// -------------------------------------------------------------------------- //

struct DateTime;

CDECCOREEXPORT stringx DateTime_Format(const DateTime& dt);

// -------------------------------------------------------------------------- //
// DateTime Value Type
// -------------------------------------------------------------------------- //

// Under linux, DateTime use clock_gettime to obtain the current time. Though it
// is not widely supported. We may use gettimeofday (or ftime) as alternative.
//
// Currently Day-light-saving time is NOT tested.

// Refer timeb under Windows
// Refer timeval, timezone under Linux
struct TimeValue
{
	INT64	Time;		// 64-bit time-stamp. Even under 32-bit Windows
	INT32	Micro;		// micro-seconds (milli-seconds precise under Windows)
	INT16	TimeZone;	// Time-zone
	INT16	DstFlag;	// DST flag (NOT tested)
};

struct DateTime
{
protected:
#ifdef X_OS_WINDOWS
	SYSTEMTIME m_st;
#else
	tm m_tm;
	UINT m_milli;
#endif

public:
	// This will get the time-stamp, it is the same with time_t
	// Return 64-bit time-stamp value under Windows 32-bit version
	inline static INT64 Timestamp();

	// This will get the time-stamp, milli-second, time-zone, and DST information
	// Refer timeb, ftime under Windows
	// Refer
	// Note that DST (day saving time) flags are NOT tested
	inline static TimeValue NowTime();

	inline static DateTime Now();

	inline static DateTime Set(int year, int month, int day, int hour, int minute, int second, int milliseconds);

#ifdef X_OS_WINDOWS
	inline static DateTime FromSystemTime(const SYSTEMTIME& st)
	{
		DateTime dt;
		dt.m_st = st;
		return dt;
	}
#else
	inline static DateTime FromTimeSpec(const timespec& ts)
	{
		DateTime dt;
		dt.m_tm = *localtime(&ts.tv_sec);
		dt.m_milli = ts.tv_nsec / 1000000;
		return dt;
	}
#endif

#ifdef X_OS_WINDOWS
	int	GetYear() const { return m_st.wYear; }
	int GetMonth() const { return m_st.wMonth; }
	int GetDay() const { return m_st.wDay; }
	int GetHour() const { return m_st.wHour; }
	int GetMinute() const { return m_st.wMinute; }
	int GetSecond() const { return m_st.wSecond; }
	int GetMilliseconds() const { return m_st.wMilliseconds; }
	int GetDayOfWeek() const { return m_st.wDayOfWeek; }
#else
	int GetYear() const { return m_tm.tm_year; }
	int GetMonth() const { return m_tm.tm_mon; }
	int GetDay() const { return m_tm.tm_mday; }
	int GetHour() const { return m_tm.tm_hour; }
	int GetMinute() const { return m_tm.tm_min; }
	int GetSecond() const { return m_tm.tm_sec; }
	int GetMilliseconds() const { return m_milli; }
	int GetDayOfWeek() const { return m_tm.tm_wday; }
#endif

	stringx	Format() { return DateTime_Format(*this); }
};

// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

inline INT64 DateTime::Timestamp()
{
	return _time64(NULL);
}

inline TimeValue DateTime::NowTime()
{
	TimeValue tv;
	__timeb64 tb;
	_ftime64(&tb);
	tv.Time = tb.time;
	tv.Micro = tb.millitm * 1000;
	tv.TimeZone = tb.timezone;
	tv.DstFlag = tb.dstflag;
	return tv;
}

inline DateTime DateTime::Now()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	return FromSystemTime(st);
}

inline DateTime DateTime::Set(int year, int month, int day, int hour, int minute, int second, int milliseconds)
{
	DateTime dt;
	dt.m_st.wYear = year;
	dt.m_st.wMonth = month;
	dt.m_st.wDay = day;
	dt.m_st.wHour = hour;
	dt.m_st.wMinute = minute;
	dt.m_st.wSecond = second;
	dt.m_st.wMilliseconds = milliseconds;

	// todo: set wDayOfWeek

	return dt;
}

#else

inline INT64 DateTime::Timestamp()
{
	return time(NULL);
}

inline TimeValue DateTime::NowTime()
{
	TimeValue val;
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	val.Time = tv.tv_sec;
	val.Micro = tv.tv_usec;
	val.TimeZone = tz.tz_minuteswest;
	val.DstFlag = tz.tz_dsttime;
	return val;
}

inline DateTime DateTime::Now()
{
	timespec tmspec;
	clock_gettime(CLOCK_REALTIME, &tmspec);
	return FromTimeSpec(tmspec);
}

inline DateTime DateTime::Set(int year, int month, int day, int hour, int minute, int second, int milliseconds)
{
	DateTime dt;
	dt.m_tm.tm_year = year;
	dt.m_tm.tm_mon = month;
	dt.m_tm.tm_mday = day;
	dt.m_tm.tm_hour = hour;
	dt.m_tm.tm_min = minute;
	dt.m_tm.tm_sec = second;
	dt.m_milli = milliseconds;

	// todo: set wDayOfWeek

	return dt;
}

#endif
// -------------------------------------------------------------------------- //

CDEC_NS_END
