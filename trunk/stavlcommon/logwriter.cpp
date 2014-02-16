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
	"",			// KLOG_INFORMATION,
	"CLAIM\t",	// KLOG_CLAIM,
	"WARN\t",	// KLOG_WARNING,
	"ERROR\t",	// KLOG_ERROR,
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
	if (file != NULL)
		m_message.append(file).append("(").append(ConverterA::ToString(line)).append("): ");
}

Logger& Logger::operator << (stringx value)
{
	m_message += g_utf8->FromUnicode(value);
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
