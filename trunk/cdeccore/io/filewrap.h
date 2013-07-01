#pragma once

#define USE_POSIX_FILEIO 1

// -------------------------------------------------------------------------- //
// File operation API wrapper
//
// Caution: Never use this class in the application code, it is only made
// for C-- library classes (such as FileStream)
// -------------------------------------------------------------------------- //

class FileWrapper
{
public:
	enum AccessMode {
		AccessRead = GENERIC_READ,
		AccessWrite = GENERIC_WRITE,
		AccessReadWrite = GENERIC_READ | GENERIC_WRITE
	};
	enum ShareMode {
		ShareExclusive = 0,
		ShareRead = FILE_SHARE_READ,
		ShareWrite = FILE_SHARE_WRITE,
		ShareReadWrite = FILE_SHARE_READ | FILE_SHARE_WRITE
	};
	enum SeekOrigin {
		SeekBegin = SEEK_SET,
		SeekCurrent = SEEK_CUR,
		SeekEnd = SEEK_END
	};

private:

#if defined(X_OS_WINDOWS)
	HANDLE	m_hFile;
#elif USE_POSIX_FILEIO
	int		m_fd;
#else
	FILE*	m_fp;
#endif

public:
	FileWrapper()
	{
	#if defined(X_OS_WINDOWS)
		m_hFile = INVALID_HANDLE_VALUE;
	#elif USE_POSIX_FILEIO
		m_fd = -1;
	#else
		m_fp = NULL;
	#endif
	}

	~FileWrapper() { Close(); }

	bool IsOpen();
	void Open(PCWSTR path, AccessMode access, ShareMode share, bool fCreate);
	UINT Read(void* buffer, UINT cbToRead);
	UINT Write(const void* buffer, DWORD cbToWrite);
	INT64 Seek(INT64 offset, SeekOrigin origin);
	INT64 Size();
	void SetLength(INT64);
	void Flush();
	void Close();
	UINT AtomRead(void* buffer, UINT cb,UINT64 pos);
	UINT AtomWrite(const void* buffer, DWORD cb,UINT64 pos);

	// Inter-operation functions
	// Do not use unless neccessary
#if defined(X_OS_WINDOWS)
	void FromHandle(HANDLE h);
	HANDLE GetHandle() { return m_hFile; }
#elif USE_POSIX_FILEIO
	void FromFD(int fd);
	int GetFD() { return m_fd; }
#else
	void FromFP(FILE* fp);
	FILE* GetFP() { return m_fp; }
#endif

};

// -------------------------------------------------------------------------- //

#if defined(X_OS_WINDOWS)

inline bool FileWrapper::IsOpen() { return m_hFile != INVALID_HANDLE_VALUE; }

#elif USE_POSIX_FILEIO

inline bool FileWrapper::IsOpen() { return m_fd != -1; }

#else

inline bool FileWrapper::IsOpen() { return m_fp != NULL; }

#endif

// -------------------------------------------------------------------------- //
