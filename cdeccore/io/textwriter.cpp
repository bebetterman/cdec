#include "stdafx.h"

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// SequenceWritingBuffer (inner)
// -------------------------------------------------------------------------- //

SequenceWritingBuffer::SequenceWritingBuffer():
m_pBuffer(NULL), m_iob(0)
{
}

SequenceWritingBuffer::SequenceWritingBuffer(ref<Stream> pStream):
m_pBuffer(NULL), m_iob(0)
{
	Open(pStream);
}

void SequenceWritingBuffer::Open(ref<Stream> stream)
{
	Close();

	m_stream = stream;
	m_pBuffer = (BYTE*)malloc(BufferSize);
	m_iob = 0;
}

void SequenceWritingBuffer::Flush()
{
	if (m_iob != 0)
	{
		m_stream->Write(m_pBuffer, m_iob);
		m_iob = 0;
		m_stream->Flush();
	}
}

void SequenceWritingBuffer::Close()
{
	if (m_stream != NULL)
	{
		if (m_iob != 0)
			m_stream->Write(m_pBuffer, m_iob);

		m_stream->Close();
		free(m_pBuffer);

		m_pBuffer = NULL;
		m_iob = 0;
		m_stream = NULL;
	}
}

// Patch, Windows.h contains a macro named min
#undef min

const BYTE* SequenceWritingBuffer::_Append(const BYTE* ps, UINT cb)
{
	if (cb == 0)
		return ps;

	cb = std::min(cb, BufferSize - m_iob);
	memcpy(m_pBuffer + m_iob, ps, cb);
	ps += cb;
	m_iob += cb;

	if (m_iob >= BufferSize)
	{
		ASSERT(m_iob == BufferSize);
		m_stream->Write(m_pBuffer, BufferSize);
		m_iob = 0;
	}

	return ps;
}

void SequenceWritingBuffer::Write(const void* buffer, UINT cb)
{
	const BYTE* pb = (const BYTE*)buffer;
	const BYTE* pe = pb + cb;

	pb = _Append(pb, (UINT)(pe - pb));

	while ((UINT)(pe - pb) >= BufferSize)
	{
		ASSERT(m_iob == 0);
		m_stream->Write(pb, BufferSize);
		pb += BufferSize;
	}

	if (pe > pb)
	{
		ASSERT(m_iob == 0);
		_Append(pb, (UINT)(pe - pb));
	}
}

// -------------------------------------------------------------------------- //
// StreawmWriter
// -------------------------------------------------------------------------- //

StreamWriter::StreamWriter(stringx filename, ref<Encoding> encoding)
{
	ref<Stream> stream = gc_new<FileStream>(filename, FileStream::AccessWrite, true);
	OpenEncoding(stream, encoding);
}

void StreamWriter::OpenEncoding(ref<Stream> stream, ref<Encoding> encoding)
{
	Close();
	m_sqb = gc_new<SequenceWritingBuffer>(stream);
	m_encoding = encoding;
}

void StreamWriter::Close()
{
	if (m_sqb != NULL)
	{
		m_sqb->Close();
		m_sqb = NULL;
		m_encoding = NULL;
	}
}

void StreamWriter::Write(stringx s)
{
	if (s == NULL)
		return;

	if (m_encoding->get_CP() == Encoding::UTF16)
	{
		m_sqb->Write(s.c_str(), (UINT)s.Length() * 2);
	}
	else
	{
		std::string sa = m_encoding->FromUnicode(s);
		m_sqb->Write(sa.data(), sa.size());
	}
}

void StreamWriter::WriteChar(WCHAR ch)
{
	switch (m_encoding->get_CP())
	{
	case Encoding::UTF16:
		m_sqb->WriteInt16(ch);
		break;
	case Encoding::UTF16BE:
	case Encoding::UTF32:
	case Encoding::UTF32BE:
		cdec_throw(Exception(EC_NotImplemented));
	default:
		if (ch >= 0x80)
			cdec_throw(Exception(EC_NotImplemented));
		m_sqb->WriteByte((BYTE)ch);
		break;
	}
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
