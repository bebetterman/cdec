#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

LogOutputConsole g_stdlogout;

ref<Encoding> g_utf8 = Encoding::get_UTF8();

const char* LMSG[] = 
{
	"LOG",	// KLOG_INFORMATION,
	"CLM",	// KLOG_CLAIM,
	"WARN",	// KLOG_WARNING,
	"ERR",	// KLOG_ERROR,
};

// -------------------------------------------------------------------------- //
// LOG Writer
// -------------------------------------------------------------------------- //

Logger::Logger(ILogOutput* out, LoggerLevel level, const char* function, const char* file, int line)
{
	char buffer[12];
	m_out = out;
	m_level = level;
	m_message = LMSG[level];
	m_message += '\t';
	if (file != NULL)
		m_message.append(file).append("(").append(itoa(line, buffer, 10)).append("): ");
}

Logger& Logger::operator << (stringx value)
{
	m_message += g_utf8->FromUnicode(value);
	return *this;
}

Logger& Logger::operator << (int value)
{
	// 0xFFFFFFFF = 4294967295
	char buffer[12];
	_itoa(value, buffer, 10);
	m_message += buffer;
	return *this;
}

Logger& Logger::operator << (UINT value)
{
	char buffer[12];
	_ultoa(value, buffer, 10);
	m_message += buffer;
	return *this;
}

Logger& Logger::operator << (INT64 value)
{
	// 0xFFFFFFFFFFFFFFFF = 18446744073709551615
	// 0x7FFFFFFFFFFFFFFF = 9223372036854775807
	char buffer[22];
	_i64toa(value, buffer, 10);
	m_message += buffer;
	return *this;
}

Logger& Logger::operator << (UINT64 value)
{
	char buffer[22];
	_ui64toa(value, buffer, 10);
	m_message += buffer;
	return *this;
}

// -------------------------------------------------------------------------- //
// LogOutputConsole
// -------------------------------------------------------------------------- //

void DebugSetOutputColor(LoggerLevel level)
{
#ifdef X_OS_WINDOWS
	static WORD attrs[] = { 7, 10, 14, 12 };
	static HANDLE _hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	static LoggerLevel lastLevel = KLOG_INFORMATION;
	if (level != lastLevel)
	{
		WORD attr = attrs[level];
		SetConsoleTextAttribute(_hStdout, attr);
		lastLevel = level;
	}
#endif
}

void LogOutputConsole::WriteMessage(const std::string& message, LoggerLevel level)
{
	DebugSetOutputColor(level);
	puts(message.c_str());
	DebugSetOutputColor(KLOG_INFORMATION);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
