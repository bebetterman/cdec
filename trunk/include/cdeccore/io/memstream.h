#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT MemoryStream: public Stream
{
	DECLARE_REF_CLASS(MemoryStream)

protected:
	std::vector<BYTE>	m_buffer;
	INT64	m_pos;

public:
	MemoryStream(int capacity = 0);
	MemoryStream(const void* data, int count, int capacity = 0);
	MemoryStream(ref<ByteArray> data, int offset, int count, int capacity = 0);
	inline ~MemoryStream() { Close(); }

	int		Read(void* pv, int count);
	int		Read2(ref<ByteArray> buffer, int offset, int count);
	int		Write(const void* pv, int count);
	int		Write2(ref<ByteArray> buffer, int offset, int count);
	INT64	Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin);

	inline void		SetLength(INT64 length) { m_buffer.resize(length); }
	inline INT64	Position() { return m_pos; }
	inline INT64	Length() { return m_buffer.size(); }

	void	Flush() {}
	void	Close() { m_buffer.clear(); }

	int		AtomRead(INT64 pos, void* buffer, int count);
	int		AtomWrite(INT64 pos, const void* buffer, int count);

	ref<ByteArray>	GetBytes();
	inline const BYTE*	GetRawBuffer() { return &m_buffer[0]; }
};

// -------------------------------------------------------------------------- //

inline int MemoryStream::Read2(ref<ByteArray> buffer, int offset, int count)
{
	if (CheckOutOfRange(offset, count, buffer->Count()))
		cdec_throw(Exception(EC_OutOfRange));

	pin_ptr<BYTE> pin = buffer->GetBuffer();
	return Read(pin.ptr() + offset, count);
}

inline int MemoryStream::Write2(ref<ByteArray> buffer, int offset, int count)
{
	if (CheckOutOfRange(offset, count, buffer->Count()))
		cdec_throw(Exception(EC_OutOfRange));

	pin_ptr<BYTE> pin = buffer->GetBuffer();
	return Write(pin.ptr() + offset, count);
}

inline INT64 MemoryStream::Seek(INT64 pos, SeekPosition cp)
{
	if (cp == Stream::SeekBegin)
		m_pos = pos;
	else if (cp == Stream::SeekCurrent)
		m_pos += pos;
	else if (cp == Stream::SeekEnd)
		m_pos = m_buffer.size() + pos;
	else
		cdec_throw(Exception(EC_InvalidArg));
	return m_pos;
}

inline ref<ByteArray> MemoryStream::GetBytes()
{
	if (m_buffer.size() == 0)
		return gc_new<ByteArray>(0);
	return gc_new<ByteArray>(&m_buffer[0], m_buffer.size());
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
