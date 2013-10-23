#pragma once

#ifdef X_OS_WINDOWS
#	include <sys/timeb.h>
#else
#	include <sys/time.h>
#endif

CDEC_NS_BEGIN

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

struct CDECCOREEXPORT DateTime
{
protected:
#ifdef X_OS_WINDOWS
	SYSTEMTIME m_st;
#else
	tm m_tm;
	UINT m_milli;
#endif

public:
	void	Set(int year, int month, int day, int hour, int minute, int second, int milliseconds);
	void	SetLocal(INT64 timestamp, int milli);
	void	SetUtc(INT64 timestamp, int milli);

#ifdef X_OS_WINDOWS
	inline void SetSystemTime(const SYSTEMTIME& st) { m_st = st; }
	inline const SYSTEMTIME& ToSystemTime() const { return m_st; }
#else
	inline void	SetTimeSpecLocal(const timespec& ts) { SetLocal(ts.tv_sec, ts.tv_nsec / 1000000); }
	inline void	SetTimeSpecUtc(const timespec& ts) { SetUtc(ts.tv_sec, ts.tv_nsec / 1000000); }
#endif

	INT64	ToTimestamp();

	static	DateTime	Now();
	static	DateTime	NowUtc();

	// This will get the time-stamp, it is the same with time_t
	// Return 64-bit time-stamp value under Windows 32-bit version
	static	INT64		NowTimestamp();

	// This will get the time-stamp, milli-second, time-zone, and DST information
	// Refer timeb, ftime under Windows
	// Refer
	// Note that DST (day saving time) flags are NOT tested
	static	TimeValue	NowTimeValue();

#ifdef X_OS_WINDOWS
	inline int GetYear() const { return m_st.wYear; }
	inline int GetMonth() const { return m_st.wMonth; }
	inline int GetDay() const { return m_st.wDay; }
	inline int GetHour() const { return m_st.wHour; }
	inline int GetMinute() const { return m_st.wMinute; }
	inline int GetSecond() const { return m_st.wSecond; }
	inline int GetMilliseconds() const { return m_st.wMilliseconds; }
	// inline int GetDayOfWeek() const { return m_st.wDayOfWeek; }
#else
	inline int GetYear() const { return m_tm.tm_year; }
	inline int GetMonth() const { return m_tm.tm_mon; }
	inline int GetDay() const { return m_tm.tm_mday; }
	inline int GetHour() const { return m_tm.tm_hour; }
	inline int GetMinute() const { return m_tm.tm_min; }
	inline int GetSecond() const { return m_tm.tm_sec; }
	inline int GetMilliseconds() const { return m_milli; }
	// inline int GetDayOfWeek() const { return m_tm.tm_wday; }
#endif

	stringx	Format();
};

// -------------------------------------------------------------------------- //

inline INT64 DateTime::NowTimestamp()
{
#ifdef X_OS_WINDOWS
	return _time64(NULL);
#else
	return time(NULL);
#endif
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
