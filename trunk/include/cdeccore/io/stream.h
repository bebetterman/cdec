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
	virtual int		Read(void* buffer, int count) = 0;
	virtual int		Read2(ref<ByteArray> buffer, int offset, int count) = 0;
	virtual int		Write(const void* buffer, int count) = 0;
	virtual int		Write2(ref<ByteArray> buffer, int offset, int count) = 0;
	virtual INT64	Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin) = 0;
	virtual void	SetLength(INT64 length) = 0;
	virtual INT64	Length() = 0;
	virtual void	Close() = 0;

	virtual int		AtomRead(INT64 pos, void* buffer, int count) = 0;
	virtual int		AtomWrite(INT64 pos, const void* buffer, int count) = 0;

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
	FileWrapper*	m_pFile;

public:
	FileStream(stringx filename, AccessMode accs, bool fCreate): m_pFile(NULL)
	{
		Open(filename, accs, fCreate);
	}

	FileStream(stringx filename, AccessMode accs, ShareMode share, bool fCreate): m_pFile(NULL)
	{
		Open(filename, accs, share, fCreate);
	}

	int		Read(void* pv, int count);
	int		Read2(ref<ByteArray> buffer, int offset, int count);
	int		Write(const void* pv, int count);
	int		Write2(ref<ByteArray> buffer, int offset, int count);
	INT64	Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin);
	void	SetLength(INT64 length);
	INT64	Length();

	void	Close();

	int		AtomRead(INT64 pos, void* buffer, int count);
	int		AtomWrite(INT64 pos, const void* buffer, int count);
	 
	FileWrapper* __Inner() { return m_pFile; }

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
