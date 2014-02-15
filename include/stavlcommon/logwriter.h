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

};

// -------------------------------------------------------------------------- //

struct STAVLCOMMONEXPORT Logger
{
	std::string m_message;

	Logger(LoggerLevel level, const char* function, const char* file, int line);

	~Logger();

	Logger& operator << (stringx value);

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

	Logger& operator << (int value);

	Logger& operator << (UINT value);

	Logger& operator << (INT64 value);

	Logger& operator << (UINT64 value);

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

CDEC_NS_END
