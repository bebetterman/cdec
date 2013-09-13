#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

MemoryStream::MemoryStream(int capacity): m_pos(0)
{
	if (capacity != 0)
		m_buffer.reserve(capacity);
}

MemoryStream::MemoryStream(const void* data, int count, int capacity): m_pos(0)
{
	if (capacity != 0)
		m_buffer.reserve(capacity);

	if (count != 0)
	{
		m_buffer.resize(count);
		memcpy(&m_buffer[0], data, count);
	}
}

MemoryStream::MemoryStream(ref<ByteArray> buffer, int offset, int count, int capacity): m_pos(0)
{
	if (CheckOutOfRange(offset, count, buffer->Count()))
		cdec_throw(Exception(EC_OutOfRange));

	if (capacity != 0)
		m_buffer.reserve(capacity);

	if (count != 0)
	{
		pin_ptr<BYTE> pin = buffer->GetBuffer();
		m_buffer.resize(count);
		memcpy(&m_buffer[0], pin.ptr() + offset, count);
	}
}

int	MemoryStream::Read(void* pv, int count)
{
	if (count < 0)
		cdec_throw(Exception(EC_InvalidArg));
	if (m_pos < 0)
		cdec_throw(IOException(EC_IO_NegativeSeek));
	if (m_pos >= m_buffer.size())
		return 0;	// No more data to read

	int cbop = (int)Math::Min((INT64)m_buffer.size() - m_pos, (INT64)count);
	memcpy(pv, &m_buffer[m_pos], cbop);
	m_pos += cbop;
	return cbop;
}

int	MemoryStream::Write(const void* pv, int count)
{
	if (count < 0)
		cdec_throw(Exception(EC_InvalidArg));
	if (m_pos < 0)
		cdec_throw(IOException(EC_IO_NegativeSeek));

	INT64 posNew = m_pos + count;
	if (posNew > m_buffer.size())
		m_buffer.resize(posNew);
	memcpy(&m_buffer[m_pos], pv, count);
	m_pos = posNew;
	return count;
}

int MemoryStream::AtomRead(INT64 pos, void* buffer, int count)
{
	cdec_throw(Exception(EC_NotImplemented));
}

int	MemoryStream::AtomWrite(INT64 pos, const void* buffer, int count)
{
	cdec_throw(Exception(EC_NotImplemented));
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
