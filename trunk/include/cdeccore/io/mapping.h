#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class FileMappingImpl;

class CDECCOREEXPORT FileMapping: public Object
{
	DECLARE_REF_CLASS(FileMapping)

protected:
	FileMappingImpl*	m_impl;
	void*	m_ptr;
	INT64	m_length;

public:
	FileMapping(): m_impl(NULL), m_ptr(NULL), m_length(0) {}

	// If length is 0, a region from position to the end of file is mapped
	// If pBaseAddress is NULL, the operating system chooses the mapping address
	void*	Open(ref<FileStream> fs, bool fWritable, INT64 position, INT64 length = 0, void* pBaseAddress = NULL);

	void*	GetAddress() { return m_ptr; }
	INT64	GetLength() { return m_length; }

	void	Flush(void* p);

	void	Close();
};

// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

class CDECCOREEXPORT FileMappingWin32: public Object
{
	DECLARE_REF_CLASS(FileMappingWin32)

protected:
	FileMappingImpl*	m_impl;

public:
	// If length is 0, a mapping of the current file is created
	// If name is NULL, an unnamed mapping object is created
	FileMappingWin32(ref<FileStream> fs, bool fWritable, INT64 length = 0, stringx name = NULL);

	// If length is 0, a region from position to end of file is mapped
	// If pBaseAddress is NULL, the operating system chooses the mapping address
	void*	MapView(INT64 position, UINT length, bool fWritable, void* pBaseAddress = NULL);

	void	UnmapView(void* p);
	void	Flush(void* p);
	void	Close();
};

#endif
// -------------------------------------------------------------------------- //
CDEC_NS_END
