#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// SequenceWritingBuffer (inner)
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT SequenceWritingBuffer : public Object
{
	DECLARE_REF_CLASS(SequenceWritingBuffer)

protected:
	const static UINT BufferSize = 4096;

	ref<Stream>	m_stream;
	BYTE*		m_pBuffer;
	UINT		m_iob;

public:
	SequenceWritingBuffer();
	SequenceWritingBuffer(ref<Stream> pStream);
	~SequenceWritingBuffer() { Close(); }

	void	Open(ref<Stream> pStream);
	void	Flush();
	void	Close();

	void	Write(const void* buffer, UINT cb);

	void	WriteByte(BYTE value) { Write(&value, sizeof(value)); }
	void	WriteInt16(UINT16 value) { Write(&value, sizeof(value)); }
	void	WriteInt32(UINT32 value) { Write(&value, sizeof(value)); }

private:
	const BYTE* _Append(const BYTE* ps, UINT cb);
};

// -------------------------------------------------------------------------- //
// TextWriter (abstract)
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT TextWriter : public Object
{
	DECLARE_REF_CLASS(TextWriter)

protected:
	TextWriter() {}

public:
	virtual void Write(stringx value) = 0;
	virtual void WriteChar(WCHAR value) = 0;
	virtual void Flush() = 0;
	virtual void Close() = 0;

	void	WriteLine(stringx value) { Write(value); WriteChar('\n'); }
	void	WriteLine() { WriteChar('\n'); }
};

// -------------------------------------------------------------------------- //
// StringWriter (TextWriter to a StringBuilder object)
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT StringWriter : public TextWriter
{
	DECLARE_REF_CLASS(StringWriter)

protected:
	ref<StringBuilder> m_sb;

public:
	StringWriter() { m_sb = gc_new<StringBuilder>(); }

	void Write(stringx value) { m_sb->Append(value); }
	void WriteChar(WCHAR value) { m_sb->Append(value); }
	void Flush() {}
	void Close() {}

	ref<StringBuilder> GetStringBuilder() { return m_sb; }
};

// -------------------------------------------------------------------------- //
// StreawmWriter
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT StreamWriter : public TextWriter
{
	DECLARE_REF_CLASS(StreamWriter)

protected:
	ref<SequenceWritingBuffer>	m_sqb;
	ref<Encoding>	m_encoding;

	void	OpenEncoding(ref<Stream> stream, ref<Encoding> encoding);

public:
	// 打开文本文件，使用指定的编码
	StreamWriter(stringx filename, ref<Encoding> encoding);

	// 写入给定流，使用指定编码
	StreamWriter(ref<Stream> pStream, ref<Encoding> encoding) { OpenEncoding(pStream, encoding); }

	~StreamWriter() { Close(); }

	void Write(stringx line);
	void WriteChar(WCHAR ch);

	ref<Encoding> get_Encoding() const { return m_encoding; }
	void ChangeEncoding(ref<Encoding> encoding) { m_encoding = encoding; }

	void Flush() { m_sqb->Flush(); }
	void Close();
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
