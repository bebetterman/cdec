#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

class BitConverter: public Object
{
	DECLARE_REF_CLASS(BitConverter)

protected:
	template<typename T>
	inline static T	Read(ref<ByteArray> buffer, int offset, T*);

	template<typename T>
	inline static void Write(ref<ByteArray> buffer, int offset, T value);

public:
	static INT16	ReadInt16(ref<ByteArray> buffer, int offset)	{ return Read<INT16>(buffer, offset, (INT16*)NULL); }
	static UINT16	ReadUInt16(ref<ByteArray> buffer, int offset)	{ return Read<UINT16>(buffer, offset, (UINT16*)NULL); }

	static INT32	ReadInt32(ref<ByteArray> buffer, int offset)	{ return Read<INT32>(buffer, offset, (INT32*)NULL); }
	static UINT32	ReadUInt32(ref<ByteArray> buffer, int offset)	{ return Read<UINT32>(buffer, offset, (UINT32*)NULL); }

	static INT64	ReadInt64(ref<ByteArray> buffer, int offset)	{ return Read<INT64>(buffer, offset, (INT64*)NULL); }
	static UINT64	ReadUInt64(ref<ByteArray> buffer, int offset)	{ return Read<UINT64>(buffer, offset, (UINT64*)NULL); }

	static void		WriteInt16(ref<ByteArray> buffer, UINT offset, INT16 value) { Write<INT16>(buffer, offset, value); }
	static void		WriteUInt16(ref<ByteArray> buffer, UINT offset, UINT16 value) { Write<UINT16>(buffer, offset, value); }

	static void		WriteInt32(ref<ByteArray> buffer, UINT offset, INT32 value) { Write<INT32>(buffer, offset, value); }
	static void		WriteUInt32(ref<ByteArray> buffer, UINT offset, UINT32 value) { Write<UINT32>(buffer, offset, value); }

	static void		WriteInt64(ref<ByteArray> buffer, UINT offset, INT64 value) { Write<INT64>(buffer, offset, value); }
	static void		WriteUInt64(ref<ByteArray> buffer, UINT offset, UINT64 value) { Write<UINT64>(buffer, offset, value); }

	static INT16	ExchangeInt16(INT16 value) { return (INT16)ExchangeUInt16((UINT16)value); }
	static UINT16	ExchangeUInt16(UINT16 value);

	static INT32	ExchangeInt32(INT32 value) { return (INT32)ExchangeUInt32((UINT32)value); }
	static UINT32	ExchangeUInt32(UINT32 value);

	static INT64	ExchangeInt64(INT64 value) { return (INT64)ExchangeUInt64((UINT64)value); }
	static UINT64	ExchangeUInt64(UINT64 value);
};

// -------------------------------------------------------------------------- //

template<typename T>
inline T BitConverter::Read(ref<ByteArray> buffer, int offset, T*)
{
	if (offset < 0 || offset + sizeof(T) > buffer->Count())
		cdec_throw(Exception(EC_OutOfRange));
	return *(T*)((BYTE*)buffer->GetBuffer().ptr() + offset);
}

template<typename T>
inline void BitConverter::Write(ref<ByteArray> buffer, int offset, T value)
{
	if (offset < 0 || offset + sizeof(T) > buffer->Count())
		cdec_throw(Exception(EC_OutOfRange));
	*(T*)((BYTE*)buffer->GetBuffer().ptr() + offset) = value;
}

inline UINT16 BitConverter::ExchangeUInt16(UINT16 value)
{
	UINT16 r;
	UINT8* p = (UINT8*)&r;
	p[0] = value >> 8;
	p[1] = value & 0xFFU;
	return r;
}

inline UINT32 BitConverter::ExchangeUInt32(UINT32 value)
{
	UINT32 r;
	UINT16* p = (UINT16*)&r;
	p[0] = ExchangeUInt16(value >> 16);
	p[1] = ExchangeUInt16(value & 0xFFFFU);
	return r;
}

inline UINT64 BitConverter::ExchangeUInt64(UINT64 value)
{
	UINT64 r;
	UINT32* p = (UINT32*)&r;
	p[0] = ExchangeUInt32(value >> 32);
	p[1] = ExchangeUInt32(value & 0xFFFFFFFFU);
	return r;
}

// -------------------------------------------------------------------------- //

CDEC_NS_END
