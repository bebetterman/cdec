#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Sequenced reading buffer
//
// SequenceReadingBuffer 为流类型提供顺序读的缓存支持。
//
// 目前的缓存策略比较简单，就是缓存当前索取的，以 4KB 为单位的文件段。这种方式适
// 用于顺序读取的文件。
// 虽然 SequenceReadingBuffer 支持定位（Seeking），但是其缓存策略并没有为此优化，
// 因此应当避免使用。
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT SequenceReadingBuffer : public Object
{
	DECLARE_REF_CLASS(SequenceReadingBuffer)

protected:
	enum { BufferSize = 4096 };

	ref<Stream>	m_pStream;
	BYTE*		m_pBuffer;
	INT64		m_pos;			// 在流中的位置，必须为 BufferSize 的整数倍
	UINT		m_eob, m_iob;	// Buffer 中的长度和 Buffer 中的游标

public:
	SequenceReadingBuffer();
	SequenceReadingBuffer(ref<Stream> pStream);
	~SequenceReadingBuffer() { Close(); }

	void Open(ref<Stream> pStream);
	void Close();

	UINT	Read(void* pv, UINT cb);
	INT64	Seek(INT64 pos, Stream::SeekPosition cp);
	INT64	Size() { return m_pStream->Length(); }
	INT64	Pos() { return m_pos + m_iob; }
	bool	IsEnd() {
		ASSERT( (m_iob >= m_eob) == (Pos() >= Size()) );
		return m_iob >= m_eob;
	}

	// 短数据快捷方法
	//
	// 当流指针（Pos 属性）与要获取的数据类型对齐时，下列方法可以获得更好的性能。
	// 如果读取越过文件末尾，将触发异常。
	// 由于 IsEnd 效率较高，为避免触发异常，请在每次读取之前使用 IsEnd 查询。
	BYTE	ReadByte();
	WORD	ReadInt16();
	DWORD	ReadInt32();

protected:
	void	ValidateBuffer(INT64 pos);
	// 为了迁就 VC 6 的做法
	void	PeekValue(DWORD* pValue, UINT typeSize);
};

// -------------------------------------------------------------------------- //
// TextReader
// -------------------------------------------------------------------------- //

interface TextReader : public Object
{
	DECLARE_REF_CLASS(TextReader)

	virtual void Reset() = 0;
	virtual bool IsEnd() = 0;

	// Exception raised when exceeding the end, to avoid exception, use IsEnd before
	virtual WCHAR ReadChar() = 0;

	virtual stringx ReadLine() = 0;
	virtual stringx ReadToEnd() = 0;

	virtual void Close() = 0;
};

// -------------------------------------------------------------------------- //
// StreamReader
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT StreamReader : public TextReader
{
	DECLARE_REF_CLASS(StreamReader)

protected:
	ref<SequenceReadingBuffer>	m_sqb;
	ref<Encoding>	m_encoding;
	UINT	m_posText;	// 文本正式开始的位置

protected:
	void	OpenNoEncoding(ref<Stream> pStream);
	void	OpenEncoding(ref<Stream> pStream, ref<Encoding> encoding);

	UINT	DetectCP();

	bool	FetchLineWideChars(std::wstring16&);
	bool	FetchLineMultiBytes(std::string&);

	WCHAR	ReadCharWide() { return m_sqb->ReadInt16(); }
	WCHAR	ReadCharMultiBytes();

	void	ReadToEndWideChar(std::wstring16& s);
	void	ReadToEndMultiBytes(std::wstring16& s);

public:
	// 打开文本文件，使用自动检测的编码
	explicit StreamReader(stringx filename);

	// 打开文本文件，使用指定的编码。如果编码错误，将导致打开失败。
	StreamReader(stringx filename, ref<Encoding> encoding);

	// 从给定流中读取文本，使用自动检测的编码
	explicit StreamReader(ref<Stream> pStream) {
		m_sqb = gc_new<SequenceReadingBuffer>();
		OpenNoEncoding(pStream);
	}

	// 从给定流中读取文本，使用指定的编码。如果编码错误，将导致打开失败
	StreamReader(ref<Stream> pStream, ref<Encoding> encoding) {
		m_sqb = gc_new<SequenceReadingBuffer>();
		OpenEncoding(pStream, encoding);
	}

	~StreamReader() { Close(); }
	void Close();

	bool IsEnd() { return m_sqb->IsEnd(); }
	void Reset() { m_sqb->Seek(m_posText, Stream::SeekBegin); }

	WCHAR ReadChar() { return m_encoding->IsWideChar() ? ReadCharWide() : ReadCharMultiBytes(); }
	stringx ReadLine();
	stringx ReadToEnd();

	ref<Encoding> get_Encoding() const { return m_encoding; }
	void ChangeEncoding(ref<Encoding> encoding) { m_encoding = encoding; }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
