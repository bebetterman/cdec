// -------------------------------------------------------------------------- //
// LOG Writer
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

enum LoggerLevel
{
	KLOG_INFORMATION,
	KLOG_CLAIM,
	KLOG_WARNING,
	KLOG_ERROR,
};

// -------------------------------------------------------------------------- //

interface ILogOutput
{
	virtual void WriteMessage(const std::string& message, LoggerLevel level) = 0;
};

// -------------------------------------------------------------------------- //

struct STAVLCOMMONEXPORT Logger
{
	ILogOutput* m_out;
	LoggerLevel	m_level;
	std::string m_message;

	Logger(ILogOutput* out, LoggerLevel level, const char* function, const char* file, int line);

	inline ~Logger()
	{
		m_out->WriteMessage(m_message, m_level);
	}

	Logger& operator << (stringx value);

	inline Logger& operator << (char value)
	{
		m_message.append(1, value);
		return *this;
	}

	inline Logger& operator << (const char* value)
	{
		m_message.append(value);
		return *this;
	}

	inline Logger& operator << (const std::string& value)
	{
		m_message.append(value);
		return *this;
	}

	inline Logger& operator << (int value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (UINT value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (INT64 value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (UINT64 value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (long value)
	{
		return (operator <<)((int)value);
	}

	inline Logger& operator << (unsigned long value)
	{
		return (operator <<)((unsigned int)value);
	}
};

// -------------------------------------------------------------------------- //

class STAVLCOMMONEXPORT LogOutputConsole : public ILogOutput
{
public:
	void WriteMessage(const std::string& message, LoggerLevel level);
};

extern STAVLCOMMONEXPORT LogOutputConsole g_stdlogout;

// -------------------------------------------------------------------------- //

#if 1

#define KLOG_(out)		Logger(out, KLOG_INFORMATION, __FUNCTION__, __FILE__, __LINE__)
#define KCLAIM_(out)	Logger(out, KLOG_CLAIM, __FUNCTION__, __FILE__, __LINE__)
#define KWARN_(out)		Logger(out, KLOG_WARNING, __FUNCTION__, __FILE__, __LINE__)
#define KERR_(out)		Logger(out, KLOG_ERROR, __FUNCTION__, __FILE__, __LINE__)

#define KLOGT_(out)		Logger(out, KLOG_INFORMATION, NULL, NULL, 0)
#define KCLAIMT_(out)	Logger(out, KLOG_CLAIM, NULL, NULL, 0)
#define KWARNT_(out)	Logger(out, KLOG_WARNING, NULL, NULL, 0)
#define KERRT_(out)		Logger(out, KLOG_ERROR, NULL, NULL, 0)

#else

struct Logger
{
	template<typename T>
	Logger& operator << (T) { return *this; }
};

#define KLOG_(out)		Logger()
#define KCLAIM_(out)	Logger()
#define KWARN_(out)		Logger()
#define KERR_(out)		Logger()

#define KLOGT_(out)		Logger()
#define KCLAIMT_(out)	Logger()
#define KWARNT_(out)	Logger()
#define KERRT_(out)		Logger()

#endif

#define KLOG	KLOG_(&g_stdlogout)
#define KCLAIM	KCLAIM_(&g_stdlogout)
#define KWARN	KWARN_(&g_stdlogout)
#define KERR	KERR_(&g_stdlogout)

#define KLOGT	KLOGT_(&g_stdlogout)
#define KCLAIMT	KCLAIMT_(&g_stdlogout)
#define KWARNT	KWARNT_(&g_stdlogout)
#define KERRT	KERRT_(&g_stdlogout)

// -------------------------------------------------------------------------- //

CDEC_NS_END
