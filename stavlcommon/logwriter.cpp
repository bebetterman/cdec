#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// LOG Writer
// -------------------------------------------------------------------------- //

Logger::Logger(LoggerLevel level, const char* function, const char* file, int line)
{
}

Logger::~Logger()
{
}

Logger& Logger::operator << (stringx value)
{
	return *this;
}

Logger& Logger::operator << (int value)
{
	return *this;
}

Logger& Logger::operator << (UINT value)
{
	return *this;
}

Logger& Logger::operator << (INT64 value)
{
	return *this;
}

Logger& Logger::operator << (UINT64 value)
{
	return *this;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
