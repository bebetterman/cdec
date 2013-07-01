#pragma once

CDEC_NS_BEGIN

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
	inline DateTime(const SYSTEMTIME& st): m_st(st)
	{
	}
#else
	tm m_tm;
	UINT m_milli;
	inline DateTime(const timespec& ts)
	{
		m_tm = *localtime(&ts.tv_sec);
		m_milli = ts.tv_nsec / 1000000;
	}
#endif

public:
	static DateTime Now();

#ifdef X_OS_WINDOWS
	int	GetYear() const { return m_st.wYear; }
	int GetMonth() const { return m_st.wMonth; }
	int GetDay() const { return m_st.wDay; }
	int GetHour() const { return m_st.wHour; }
	int GetMinute() const { return m_st.wMinute; }
	int GetSecond() const { return m_st.wSecond; }
	int GetMillisecond() const { return m_st.wMilliseconds; }
	int GetDayOfWeek() const { return m_st.wDayOfWeek; }
#else
	int GetYear() const { return m_tm.tm_year; }
	int GetMonth() const { return m_tm.tm_mon; }
	int GetDay() const { return m_tm.tm_mday; }
	int GetHour() const { return m_tm.tm_hour; }
	int GetMinute() const { return m_tm.tm_min; }
	int GetSecond() const { return m_tm.tm_sec; }
	int GetMillisecond() const { return m_milli; }
	int GetDayOfWeek() const { return m_tm.tm_wday; }
#endif
};

// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

inline DateTime DateTime::Now()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	return DateTime(st);
}

#else

inline DateTime DateTime::Now()
{
	timespec tmspec;
    clock_gettime(CLOCK_REALTIME, &tmspec);
    return DateTime(tmspec);
}

#endif
// -------------------------------------------------------------------------- //

CDEC_NS_END
