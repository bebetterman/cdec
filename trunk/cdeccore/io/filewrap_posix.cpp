#include "stdafx.h"
#include "filewrap.h"

#if !defined(X_OS_WINDOWS)

#include <unistd.h>
#include <fcntl.h>

void FileWrapper::Open(PCWSTR path, AccessMode access, ShareMode share, bool fCreate)
{
	ASSERT(!IsOpen() && path != NULL && *path != 0);

	size_t len = wstrlen16(path);
	std::string patha = cdec::Encoding::get_Default()->FromUnicode(path, len);

	int oflag = 0;
	switch (access)
	{
	case AccessMode::AccessRead:
		oflag |= O_RDONLY;
		break;
	case AccessMode::AccessWrite:
		oflag |= O_WRONLY;
		break;
	case AccessMode::AccessReadWrite:
		oflag |= O_RDWR;
		break;
	default:
		cdec_throw(IOException(EC_InvalidArg));
	}

	if (fCreate)
		m_fd = open(patha.c_str(), oflag | O_CREAT, S_IRWXU);
	else
		m_fd = open(patha.c_str(), oflag);

	if (m_fd == -1)
		cdec_throw_stdc_lasterr(IOException);
}

void FileWrapper::FromFD(int fd)
{
	ASSERT(!IsOpen() && fd != -1);
	m_fd = fd;
}

void FileWrapper::Close()
{
	if (m_fd != -1)
	{
		close(m_fd);
		m_fd = -1;
	}
}

int FileWrapper::Read(void* buffer, int cbToRead)
{
	ASSERT(m_fd != -1 && buffer != NULL);
	int cbRead = read(m_fd, buffer, cbToRead);
	if (cbRead == -1)
		cdec_throw_stdc_lasterr(IOException);
	return cbRead;
}

int FileWrapper::AtomRead(INT64 pos, void* buffer, int cbToRead)
{
	ASSERT(m_fd != -1 && buffer != NULL);
	int cbRead = pread(m_fd, buffer, cbToRead, pos);
	if (cbRead == -1)
		cdec_throw_stdc_lasterr(IOException);
	return cbRead;
}

int FileWrapper::Write(const void* buffer, int cbToWrite)
{
	ASSERT(m_fd != -1 && buffer != NULL);
	int cbWritten = write(m_fd, buffer, cbToWrite);
	if (cbWritten == -1)
		cdec_throw_stdc_lasterr(IOException);
	return cbWritten;
}

int FileWrapper::AtomWrite(INT64 pos, const void* buffer, int cbToWrite)
{
	ASSERT(m_fd != -1 && buffer != NULL);
	int cbWritten = pwrite(m_fd, buffer, cbToWrite, pos);
	if (cbWritten == -1)
		cdec_throw_stdc_lasterr(IOException);
	return cbWritten;
}

INT64 FileWrapper::Seek(INT64 offset, SeekOrigin origin)
{
	ASSERT(m_fd != -1);
	INT64 newpos = lseek64(m_fd, offset, origin);
	if (newpos == -1)
		cdec_throw_stdc_lasterr(IOException);
	return newpos;
}

INT64 FileWrapper::Size()
{
	ASSERT(m_fd != -1);
	INT64 curpos = Seek(0, SeekOrigin::SeekCurrent);
	INT64 len = Seek(0, SeekOrigin::SeekEnd);
	Seek(curpos, SeekOrigin::SeekBegin);
	return len;
}

void FileWrapper::SetLength(INT64 len)
{
	ASSERT(IsOpen());
	INT64 cursize = Size();
#if 1
	// Performance lower but cause a non-sparse file
	if (len > cursize)
	{
		// For a pre-allocation behavior
#	if 1
		int err = posix_fallocate(m_fd, 0, len);
		if (err != 0)
		{
			switch (err)
			{
				case EBADF: puts("EBADF"); break;
				case EFBIG: puts("EFBIG"); break;
				case EINVAL: puts("EINVAL"); break;
				case ENODEV: puts("ENODEV"); break;
				case ENOSPC: puts("ENOSPC"); break;
				case ESPIPE: puts("ESPIPE"); break;
				default: printf("%d\n", err); break;
			}
			cdec_throw_stdc_lasterr(IOException);
		}
#	else
		const int bufferSize = 0x100000 * 64;
		BYTE* buffer = new BYTE[bufferSize];
		memset(buffer, 0, bufferSize);
		Seek(cursize, SeekBegin);
		long remain = len - cursize;
		while (remain > bufferSize)
		{
			Write(buffer, bufferSize);
			remain -= bufferSize;
		}
		Write(buffer, (int)remain);
		free(buffer);
#	endif
	}
	else if (len < cursize)
#endif
	{
		if (ftruncate(m_fd, len))
			cdec_throw_stdc_lasterr(IOException);
	}
}

void FileWrapper::Flush()
{
	ASSERT(IsOpen());
	// TODO Not implemented
}

#endif
