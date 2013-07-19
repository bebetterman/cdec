#pragma once

// -------------------------------------------------------------------------- //
// Error Code and Exception
// -------------------------------------------------------------------------- //

#define MAKE_EX_CODE(ns, code) ((ns << 16) | code)

enum
{
	// Namespaces
	ECNS_General = 0,				// General exceptions
	ECNS_IO,						// IO exceptions
	ECNS_CRYPT,						// Crypto exceptions
	ECNS_XML,						// XML exceptions
	ECNS_JSON,						// Json exceptions
	ECNS_App = 0x4000,				// Application specified exceptions

	// HRESULT related
	EC_GENERAL_ST = MAKE_EX_CODE(ECNS_General, 0),
	EC_Fail,						// E_FAIL, Unspecified error
	EC_Unexpected,					// E_UNEXPECTED, Catastrophic failure
	EC_NotImplemented,				// E_NOTIMPL, Not implemented
	EC_OutOfMemory,					// E_OUTOFMEMORY, Ran out of memory
	EC_InvalidArg,					// E_INVALIDARG, One or more arguments are invalid
	EC_NoInterface,					// E_NOINTERFACE, No such interface supported
	EC_InvalidPtr,					// E_POINTER, Invalid pointer
	EC_InvalidHandle,				// E_HANDLE, Invalid handle
	EC_Aborted,						// E_ABORT, Operation aborted
	EC_AccessDenied,				// E_ACCESSDENIED, General access denied error

	// Additional general errors
	EC_OutOfRange,					// Index out of range
	EC_OutOfBuffer,					// Data out of buffer
	EC_Overflow,					// Math operation overflow
	EC_InvalidValue,				// Value is invalid
	EC_KeyConflict,					// Key exists in a set/map
	EC_KeyNotFound,					// Specified key not found
	EC_OperationUnsupported,		// Unsupported operation

	// IOException
	EC_IO_ST = MAKE_EX_CODE(ECNS_IO, 0),
	EC_IO_FileNotFound,				// File or folder not found
	EC_IO_PathNotFound,				// Path not found
	EC_IO_DiskFull,					// Disk full
	EC_IO_NegativeSeek,				// Negative seek position
	EC_IO_ShareViolation,			// Sharing violation
	EC_IO_EndOfStream,				// Beyond end of stream
	EC_IO_AlreadyOpened,			// File already opened
	EC_IO_NotOpened,				// File not opened
	EC_IO_ReadWriteFail,			// Read/Write fail
	EC_IO_AlreadyExists,			// File or directory already exists
};

struct CDECEGGEXPORT Exception
{
	int Code;
	Exception(int code): Code(code) {}

	static int FromWindowsErr(DWORD err);
	static int FromStdcErr(int);
};

struct IOException: Exception
{
	IOException(int code): Exception(code) {}
};

struct XmlException: Exception
{
	XmlException(int code): Exception(code) {}
};

extern bool cdec_PrintError;

template<class T>
inline T _cdec_show_exception_message(T e, const char* file, int line)
{
	if (cdec_PrintError)
		printf("Exception, At %s(%d), Code %d\n", file, line, e.Code);
	return e;
}

#define cdec_throw(e) throw _cdec_show_exception_message(e, __FILE__, __LINE__)

#define cdec_throw_win32_err(T, code) cdec_throw(T(Exception::FromWindowsErr(code)))

#define cdec_throw_win32_lasterr(T) cdec_throw_win32_err(T, GetLastError())

#define cdec_throw_stdc_err(T, code) cdec_throw(T(Exception::FromStdcErr(code)))

#define cdec_throw_stdc_lasterr(T) cdec_throw_stdc_err(T, errno)

// -------------------------------------------------------------------------- //
