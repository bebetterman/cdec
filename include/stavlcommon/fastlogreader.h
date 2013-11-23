#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //
// Fast LOG Reader
// -------------------------------------------------------------------------- //

const UINT FLR_BUFFER_SIZE = 0x1400000;	// 20MB

class STAVLCOMMONEXPORT FastLogReader: public Object
{
	DECLARE_REF_CLASS(FastLogReader)

protected:
	ref<Stream>		m_istream;
	BYTE	*m_buffer1, *m_buffer2;
	UINT	m_offset, m_size1, m_size2;

public:
	inline FastLogReader(ref<Stream> istream);	
	inline ~FastLogReader() { Close(); }

	inline bool	IsEof() { return m_offset == m_size1 && m_size2 == 0; }

	BYTE	PeekByte();
	BYTE	ReadByte();
	WCHAR	ReadChar();

	ref<StringArrayList>	ReadTextColumns(WCHAR separator);

	void	Close();

protected:
	BYTE	ReadByteExtend();
};

// -------------------------------------------------------------------------- //

inline BYTE FastLogReader::PeekByte()
{
	if (m_offset < m_size1)
		return m_buffer1[m_offset];

	return m_size2 != 0 ? m_buffer2[0] : 0;
}

inline BYTE FastLogReader::ReadByte()
{
	if (m_offset < m_size1)
		return m_buffer1[m_offset++];

	return ReadByteExtend();
}

inline WCHAR FastLogReader::ReadChar()
{
	// Supports UTF-8 only
	char ch[6];
	ch[0] = ReadByte();
	int n = Encoding::GetUtf8TrailingCount(ch[0]);
	if (n < 0)
		cdec_throw(Exception(EC_InvalidValue));

	for (int i = 1; i <= n; ++i)
		ch[i] = ReadByte();
	return Encoding::DecodeUtf8Char(ch, n + 1);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
