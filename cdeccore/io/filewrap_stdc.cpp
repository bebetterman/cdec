#include "stdafx.h"
#include "filewrap.h"

#if !defined(X_OS_WINDOWS) && !USE_POSIX_FILEIO

#	include <unistd.h>
#	include <errno.h>
#	include <fcntl.h>
#	include <stdio.h>

inline
int lock_set(int fd, int type)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_type = type;
	return fcntl(fd, F_SETLK, &lock);
}

void FileWrapper::Open(LPCWSTR path, AccessMode access, ShareMode share, bool fCreate)
{
	ASSERT(!IsOpen() && path != NULL && *path != 0);

	size_t len = wcslen(path);
    size_t mlen = wcstombs(NULL, path, 0);
	char pszPath[mlen + 1];
	wcstombs(pszPath, path, mlen);

    const char* pcszMode = NULL;

    if (fCreate)
    {
        switch (access)
        {
        case GENERIC_READ:
            pcszMode = "r"; // 如果文件不存在打开会失败，本应是不存在就创建的，
            break;          // 但是没什么意义，所以失败就是失败吧。
        case GENERIC_WRITE:
            pcszMode = "a";
            break;
        case (GENERIC_READ | GENERIC_WRITE):
            pcszMode = "a+";
            break;
        }
    }
    else
    {
        switch (access)
        {
        case GENERIC_READ:
            pcszMode = "r";
            break;
        case GENERIC_WRITE:
            pcszMode = "a";
            break;
        case (GENERIC_READ | GENERIC_WRITE):
            pcszMode = "r+";
            break;
        }
    }

    if (pcszMode == NULL)
		kfc_throw(IOException(EC_InvalidArg));

    m_fp = fopen(pszPath, pcszMode);
	if (m_fp == NULL)
		kfc_throw_stdc_lasterr(IOException);

	int fd = fileno(m_fp);
	fcntl(fd, F_SETFD, O_NONBLOCK);
	int nRC = 0;

	switch (share)
	{
	case ShareExclusive:
		nRC = lock_set(fd, F_WRLCK);
		break;
	case ShareRead:
		nRC = lock_set(fd, F_RDLCK);
		break;
	case ShareWrite:
		nRC = lock_set(fd, F_UNLCK);
		break;
	case ShareReadWrite:
		nRC = lock_set(fd, F_UNLCK);
		break;
	default:
		break;
	}
	if (nRC == -1)
		kfc_throw(IOException(EC_AccessDenied));
}

void FileWrapper::FromFP(FILE* fp)
{
	ASSERT(!IsOpen() && fp != NULL);
	m_fp = fp;
}

void FileWrapper::Close()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}

UINT FileWrapper::Read(PVOID buffer, UINT cbToRead)
{
	ASSERT(IsOpen() && buffer != NULL);
	UINT cbRead = fread(buffer, sizeof(char), cbToRead, m_fp);
	if (ferror(m_fp))
		kfc_throw_stdc_lasterr(IOException);
	return cbRead;
}

UINT FileWrapper::Write(LPCVOID buffer, DWORD cbToWrite)
{
	ASSERT(IsOpen() && buffer != NULL);
	UINT cbWritten = fwrite(buffer, sizeof(char), cbToWrite, m_fp);
	if (ferror(m_fp))
		kfc_throw_stdc_lasterr(IOException);
	return cbWritten;
}

UINT FileWrapper::AtomRead(void* buffer, UINT cbToRead,UINT64 pos)
{
	ASSERT(m_fd != -1 && buffer != NULL);
	size_t cbRead = pread(m_fd, buffer, cbToRead, pos);
	if (cbRead == -1)
		cdec_throw_stdc_lasterr(IOException);
	return cbRead;
}

UINT FileWrapper::AtomWrite(const void* buffer, DWORD cbToWrite, UINT64 pos)
{
	ASSERT(m_fd != -1 && buffer != NULL);
	size_t cbWritten = pwrite(m_fd, buffer, cbToWrite, pos);
	if (cbWritten == -1)
		cdec_throw_stdc_lasterr(IOException);
	return cbWritten;
}

INT64 FileWrapper::Seek(INT64 offset, SeekOrigin origin)
{
	ASSERT(IsOpen());
	if (fseeko64(m_fp, offset, origin))
		kfc_throw_stdc_lasterr(IOException);
	INT64 lNewPos = ftello64(m_fp);
	if (-1 == lNewPos)
		kfc_throw_stdc_lasterr(IOException);
	return lNewPos;
}

INT64 FileWrapper::Size()
{
	ASSERT(IsOpen());
	INT64 lOldPos = ftello64(m_fp);
	if (-1 ==lOldPos)
		kfc_throw_stdc_lasterr(IOException);
	if (fseeko64(m_fp, 0, SEEK_END))
		kfc_throw_stdc_lasterr(IOException);
	INT64 lFileSize = ftello64(m_fp);
	if (-1 == lFileSize)
		kfc_throw_stdc_lasterr(IOException);
	if (fseeko64(m_fp, lOldPos, SEEK_SET))
		kfc_throw_stdc_lasterr(IOException);
	return lFileSize;
}

void FileWrapper::SetLength(INT64 len)
{
	ASSERT(IsOpen());
	if (ftruncate(fileno(m_fp), len))
		kfc_throw_stdc_lasterr(IOException);
}

void FileWrapper::Flush()
{
	ASSERT(IsOpen());
	if (fflush(m_fp))
		kfc_throw_stdc_lasterr(IOException);
}

#endif
