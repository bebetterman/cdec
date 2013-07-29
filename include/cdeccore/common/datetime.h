#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Predeclaration, do not use
// -------------------------------------------------------------------------- //

struct DateTime;

CDECCOREAPI(stringx) DateTime_Format(const DateTime& dt);

// -------------------------------------------------------------------------- //
// DateTime Value Type
// -------------------------------------------------------------------------- //

// Under linux, DateTime use clock_gettime to obtain the current time. Though it
// is not widely supported. We may use gettimeofday (or ftime) as alternative.
//
// Currently Day-light-saving time is not supported.

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

inline DateTime DateTime::Now()
{
	timespec tmspec;
    clock_gettime(CLOCK_REALTIME, &tmspec);
    return FromTimeSpec(tmspec);
}

inline DateTime DateTime::Set(int year, int month, int day, int hour, int minute, int second, int milliseconds)
{
	DateTime dt;
	m_tm.tm_year = year;
	m_tm.tm_mon = month;
	m_tm.tm_mday = day;
	m_tm.tm_hour = hour;
	m_tm.tm_min = minute;
	m_tm.tm_sec = second;
	m_milli = milliseconds;

	// todo: set wDayOfWeek

	return dt;
}

#endif
// -------------------------------------------------------------------------- //

CDEC_NS_END
