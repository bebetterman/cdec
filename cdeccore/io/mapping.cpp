#include "stdafx.h"
#include "filewrap.h"

#ifndef X_OS_WINDOWS
#include <sys/mman.h>
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class FileMappingImpl
{
protected:
#ifdef X_OS_WINDOWS
	HANDLE	m_handle;
#else
	int		m_fd;
#endif

public:
	FileMappingImpl();

	// Under Windows
	//   If length is 0, a mapping of the current file is created
	//   If name is NULL, an unnamed mapping object is created
	// Under Linux
	//   Nothing to do
	void	Create(FileWrapper* fw, bool fWritable, INT64 length, stringx name);

	// Under Windows
	//   If length is 0, a region from position to the end of file is mapped
	//   If pBaseAddress is NULL, the operating system chooses the mapping address
	// Under Linux
	//   Length must be the actual size
	//   If pBaseAddress is NULL, the operating system chooses the mapping address
	void*	MapView(INT64 position, UINT length, bool fWritable, void* pBaseAddress);

	// Under Windows
	//   Must be 0
	// Under Linux
	//   Must be the actual size
	void	UnmapView(void* p, UINT length);
	void	Flush(void* p);
	void	Close();
};

#ifdef X_OS_WINDOWS

FileMappingImpl::FileMappingImpl(): m_handle(NULL)
{
}

void FileMappingImpl::Create(FileWrapper* fw, bool fWritable, INT64 length, stringx name)
{
	Close();

	HANDLE hFile = fw->GetHandle();
	if (hFile == INVALID_HANDLE_VALUE)
		cdec_throw(Exception(EC_InvalidHandle));

	DWORD* pLength = (DWORD*)&length;
	DWORD flProtect = fWritable ? PAGE_READWRITE : PAGE_READONLY;
	PCWSTR pName = name.c_str();
	HANDLE handle = CreateFileMappingW(hFile, NULL, flProtect, pLength[1], pLength[0], pName);

	if (handle != NULL)
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			m_handle = handle;
		}
		else
		{
			CloseHandle(handle);
			cdec_throw(IOException(EC_IO_AlreadyExists));
		}
	}
	else
		cdec_throw_win32_lasterr(IOException);
}

void* FileMappingImpl::MapView(INT64 position, UINT length, bool fWritable, void* pBaseAddress)
{
	if (m_handle == NULL)
		cdec_throw(IOException(EC_IO_NotOpened));

	DWORD* pPosition = (DWORD*)&position;
	DWORD access = fWritable ? FILE_MAP_WRITE : FILE_MAP_READ;
	void* p = MapViewOfFileEx(m_handle, access, pPosition[1], pPosition[0], length, pBaseAddress);
	if (p == NULL)
		cdec_throw_win32_lasterr(IOException);
	return p;
}

void FileMappingImpl::UnmapView(void* p, UINT)
{
	if (!UnmapViewOfFile(p))
		cdec_throw_win32_lasterr(IOException);
}

void FileMappingImpl::Flush(void *p)
{
	if (!FlushViewOfFile(p, 0))
		cdec_throw_win32_lasterr(IOException);
}

void FileMappingImpl::Close()
{
	if (m_handle != NULL)
	{
		CloseHandle(m_handle);
		m_handle = NULL;
	}
}

#else

FileMappingImpl::FileMappingImpl(): m_fd(-1)
{
}

void FileMappingImpl::Create(FileWrapper* fw, bool fWritable, INT64 length, stringx name)
{
	Close();

	m_fd = fw->GetFD();
	if (m_fd == -1)
		cdec_throw(Exception(EC_InvalidHandle));
}

void* FileMappingImpl::MapView(INT64 position, UINT length, bool fWritable, void* pBaseAddress)
{
	if (m_fd == -1)
		cdec_throw(IOException(EC_IO_NotOpened));

	int prot = fWritable ? PROT_READ | PROT_WRITE : PROT_READ;
	int flags = MAP_SHARED | (pBaseAddress != NULL ? MAP_FIXED : 0);
	void* p = mmap(pBaseAddress, length, prot, flags, m_fd, position);
	if (p == NULL)
		cdec_throw_stdc_lasterr(IOException);
	return p;
}

void FileMappingImpl::UnmapView(void* p, UINT length)
{
	if (munmap(p, length) != 0)
		cdec_throw_stdc_lasterr(IOException);
}

void FileMappingImpl::Flush(void *p)
{
}

void FileMappingImpl::Close()
{
	m_fd = -1;
}

#endif

// -------------------------------------------------------------------------- //

void* FileMapping::Open(ref<FileStream> fs, bool fWritable, INT64 position, INT64 length, void* pBaseAddress)
{
	Close();

#ifndef X_OS_WINDOWS
	if (length == 0)
		length = fs->Length();
#endif

	m_impl = new FileMappingImpl();
	m_impl->Create(fs->__Inner(), fWritable, length, NULL);	// Named handle not supported under Linux
	m_ptr = m_impl->MapView(position, length, fWritable, pBaseAddress);
	m_length = length;

	return m_ptr;
}

void FileMapping::Flush(void* p) { m_impl->Flush(p); }

void FileMapping::Close()
{
	if (m_ptr != NULL)
	{
		m_impl->UnmapView(m_ptr, m_length);
		m_ptr = NULL;
		m_length = 0;
	}

	if (m_impl != NULL)
	{
		m_impl->Close();
		m_impl = NULL;
	}
}

// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

FileMappingWin32::FileMappingWin32(ref<FileStream> fs, bool fWritable, INT64 length, stringx name)
{
	m_impl = new FileMappingImpl();
	m_impl->Create(fs->__Inner(), fWritable, length, name);
}

void* FileMappingWin32::MapView(INT64 position, UINT length, bool fWritable, void* pBaseAddress)
{
	return m_impl->MapView(position, length, fWritable, pBaseAddress);
}

void FileMappingWin32::UnmapView(void* p) { m_impl->UnmapView(p, 0); }

void FileMappingWin32::Flush(void* p) { m_impl->Flush(p); }

void FileMappingWin32::Close() { m_impl->Close(); }

#endif
// -------------------------------------------------------------------------- //

CDEC_NS_END
