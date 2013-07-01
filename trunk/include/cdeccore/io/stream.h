#pragma once

// -------------------------------------------------------------------------- //

#define FILE_BEGIN			0
#define FILE_CURRENT		1
#define FILE_END			2

#define GENERIC_READ		(0x80000000L)
#define GENERIC_WRITE		(0x40000000L)

#define FILE_SHARE_READ		0x00000001  
#define FILE_SHARE_WRITE	0x00000002  
#define FILE_SHARE_DELETE	0x00000004  

class FileWrapper;

// -------------------------------------------------------------------------- //

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //
class CDECCOREEXPORT Stream : public Object
{
	DECLARE_REF_CLASS(Stream)

public:
	enum SeekPosition
	{
		SeekBegin	= FILE_BEGIN,
		SeekCurrent	= FILE_CURRENT,
		SeekEnd		= FILE_END,
	};

protected:
	Stream() {}
	virtual ~Stream() {}

public:
	virtual UINT __stdcall Read(void* buffer, UINT cb) = 0;
	virtual UINT __stdcall Read(ref<ArrayV<BYTE> > buffer, size_t cbToRead, size_t offset) = 0;
	virtual UINT __stdcall Write(const void* buffer, UINT cb) = 0;
	virtual UINT __stdcall Write(ref<ArrayV<BYTE> > buffer, size_t cbToWrite, size_t offset) = 0;
	virtual INT64 __stdcall Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin) = 0;
	virtual void __stdcall SetLength(INT64 length) = 0;
	virtual INT64 __stdcall Length() = 0;
	virtual void __stdcall Close() = 0;

	virtual UINT __stdcall AtomRead(void* buffer, UINT cb,UINT64 pos) = 0;
	virtual UINT __stdcall AtomWrite(const void* buffer, UINT cb,UINT64 pos) = 0;

	INT64 Pos() { return Seek(0, SeekCurrent); }
};

class CDECCOREEXPORT FileStream: public Stream
{
public:
	enum AccessMode
	{
		AccessRead		= GENERIC_READ,
		AccessWrite		= GENERIC_WRITE,
		AccessReadWrite	= AccessRead | AccessWrite,
	};
	enum ShareMode
	{
		ShareRead		= FILE_SHARE_READ,
		ShareReadWrite	= FILE_SHARE_READ | FILE_SHARE_WRITE,
	};

protected:
	FileWrapper*		m_pFile;

public:
	FileStream(stringx filename, AccessMode accs, bool fCreate): m_pFile(NULL)
	{
		Open(filename, accs, fCreate);
	}

	FileStream(stringx filename, AccessMode accs, ShareMode share, bool fCreate): m_pFile(NULL)
	{
		Open(filename, accs, share, fCreate);
	}

	UINT __stdcall Read(void* pv, UINT cb);
	UINT __stdcall Read(ref<ArrayV<BYTE> > ByteArray, size_t cbToRead, size_t offset);
	UINT __stdcall Write(const void* pv, UINT cb);
	UINT __stdcall Write(ref<ArrayV<BYTE> > ByteArray, size_t cbToWrite, size_t offset);
	INT64 __stdcall Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin);
	void __stdcall SetLength(INT64 length);
	INT64 __stdcall Length();

	void __stdcall Close();

	UINT __stdcall AtomRead(void* buffer, UINT cb,UINT64 pos);
	UINT __stdcall AtomWrite(const void* buffer, UINT cb,UINT64 pos);
	 
	FileWrapper* __stdcall __Inner() { return m_pFile; }

protected:
	~FileStream() { Close(); }

	void Open(stringx filename, AccessMode accs, bool fCreate)
	{
		int share = ShareRead;
		if (accs == AccessRead)
			share |= ShareReadWrite;
		Open(filename, accs, (ShareMode)share, fCreate);
	}
	void Open(stringx filename, AccessMode accs, ShareMode share, bool fCreate);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
