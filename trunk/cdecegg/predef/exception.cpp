#include "stdafx.h"

// -------------------------------------------------------------------------- //
// Implement for IOException, see cdec_predef.h
// -------------------------------------------------------------------------- //

#if defined(X_OS_WINDOWS)

int Exception::FromWindowsErr(DWORD err)
{
	switch (err)
	{
	case ERROR_FILE_NOT_FOUND:
		return EC_IO_FileNotFound;
	case ERROR_PATH_NOT_FOUND:
		return EC_IO_PathNotFound;
	case ERROR_ACCESS_DENIED:
		return EC_AccessDenied;
	case ERROR_DISK_FULL:
		return EC_IO_DiskFull;
	case ERROR_NEGATIVE_SEEK:
		return EC_IO_NegativeSeek;
	case ERROR_SHARING_VIOLATION:
		return EC_IO_ShareViolation;
	case ERROR_ALREADY_EXISTS:
		return EC_IO_AlreadyExists;
	default:
		// ASSERT(false);	// Unexpected
		return EC_Unexpected;
	}
}

#else

int Exception::FromWindowsErr(DWORD err)
{
	return EC_Unexpected;
}

#endif

int Exception::FromStdcErr(int err)
{
	// TODO Mapping error codes
	return EC_Unexpected;
}
