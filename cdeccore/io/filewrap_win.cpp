#include "stdafx.h"
#include "filewrap.h"

#if defined(X_OS_WINDOWS)
// -------------------------------------------------------------------------- //

void FileWrapper::Open(LPCWSTR path, AccessMode access, ShareMode share, bool fCreate)
{
	ASSERT(!IsOpen() && path != NULL && *path != 0);
	DWORD dw = fCreate ? OPEN_ALWAYS : OPEN_EXISTING;
	m_hFile = ::CreateFileW(path, access, share, NULL, dw, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
		cdec_throw_win32_lasterr(IOException);
}

void FileWrapper::FromHandle(HANDLE hFile)
{
	ASSERT(!IsOpen() && hFile != INVALID_HANDLE_VALUE);
	m_hFile = hFile;
}

void FileWrapper::Close()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

UINT FileWrapper::Read(PVOID buffer, UINT cbToRead)
{
	ASSERT(IsOpen() && buffer != NULL);
	UINT cbRead = 0;
	if (!::ReadFile(m_hFile, buffer, cbToRead, (DWORD*)&cbRead, NULL))
		cdec_throw_win32_lasterr(IOException);
	return cbRead;
}

UINT FileWrapper::Write(LPCVOID buffer, DWORD cbToWrite)
{
	ASSERT(IsOpen() && buffer != NULL);
	UINT cbWritten = 0;
	if (!::WriteFile(m_hFile, buffer, cbToWrite, (DWORD*)&cbWritten, NULL))
		cdec_throw_win32_lasterr(IOException);
	return cbWritten;
}

UINT FileWrapper::AtomRead(void* buffer, UINT cbToRead,UINT64 pos)
{
	cdec_throw_win32_lasterr(IOException);// Not supported yet
	return -1;
}

UINT FileWrapper::AtomWrite(const void* buffer, DWORD cbToWrite, UINT64 pos)
{
	cdec_throw_win32_lasterr(IOException);// Not supported yet
	return -1;
}

INT64 FileWrapper::Seek(INT64 offset, SeekOrigin origin)
{
	ASSERT(IsOpen());
	LARGE_INTEGER liOffs, liNewPos;
	liOffs.QuadPart = offset;
	liNewPos.QuadPart = 0;
	if (!::SetFilePointerEx(m_hFile, liOffs, &liNewPos, origin))
		cdec_throw_win32_lasterr(IOException);
	return liNewPos.QuadPart;
}

INT64 FileWrapper::Size()
{
	ASSERT(IsOpen());
	LARGE_INTEGER liSize;
	liSize.QuadPart = 0;	
	if (!::GetFileSizeEx(m_hFile, &liSize))
		cdec_throw_win32_lasterr(IOException);
	return liSize.QuadPart;
}

void FileWrapper::SetLength(INT64 len)
{
	ASSERT(IsOpen());
	Seek(len, SeekOrigin::SeekBegin);
	if (!::SetEndOfFile(m_hFile))
		cdec_throw_win32_lasterr(IOException);
}

void FileWrapper::Flush()
{
	ASSERT(IsOpen());
	if (!::FlushFileBuffers(m_hFile))
		cdec_throw_win32_lasterr(IOException);
}

// -------------------------------------------------------------------------- //
#endif
