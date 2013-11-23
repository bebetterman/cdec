#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Fast LOG Reader
// -------------------------------------------------------------------------- //

BYTE FastLogReader::ReadByteExtend()
{
	ASSERT(m_offset == m_size1);
	if (m_size2 == 0)
		return 0;

	// Swap buffer
	BYTE* t = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = t;

	m_size1 = m_size2;
	m_offset = 0;

	m_size2 = m_istream->Read(m_buffer2, FLR_BUFFER_SIZE);

	return m_buffer1[m_offset++];
}

// -------------------------------------------------------------------------- //

FastLogReader::FastLogReader(ref<Stream> istream): m_istream(istream)
{
	m_istream->Seek(0, Stream::SeekBegin);
	m_buffer1 = (BYTE*)CoreAllocLC(FLR_BUFFER_SIZE);
	m_buffer2 = (BYTE*)CoreAllocLC(FLR_BUFFER_SIZE);

	m_size1 = m_istream->Read(m_buffer1, FLR_BUFFER_SIZE);
	m_size2 = m_istream->Read(m_buffer2, FLR_BUFFER_SIZE);
	m_offset = 0;
}

ref<StringArrayList> FastLogReader::ReadTextColumns(WCHAR separator)
{
	if (IsEof())
		return NULL;

	std::wstring16 seg;
	ref<StringArrayList> r = gc_new<StringArrayList>();
	while (!IsEof())
	{
		WCHAR ch = ReadChar();
		if (ch == separator)
		{
			r->Add(stringx(seg));
			seg.clear();
		}
		else if (ch == 10)	// \n
		{
			r->Add(stringx(seg));
			return r;
		}
		else if (ch == 13)	// \r, \r\n
		{
			if (PeekByte() == 10)
				ReadByte();
			r->Add(stringx(seg));
			return r;
		}
		else
			seg += ch;
	}
	r->Add(stringx(seg));
	return r;
}

void FastLogReader::Close()
{
	CoreFreeLC(m_buffer1, FLR_BUFFER_SIZE);
	CoreFreeLC(m_buffer2, FLR_BUFFER_SIZE);
	m_istream = NULL;
	m_buffer1 = m_buffer2 = NULL;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
