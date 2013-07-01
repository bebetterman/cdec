#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

CDECCOREEXPORT	void*	AesCreateCtx(const BYTE* key, int cbKey);
CDECCOREEXPORT	void	AesDeleteCtx(void* ctx);
CDECCOREEXPORT	int		AesEncode(void* ctx, const BYTE* src, BYTE* dest, int length);
CDECCOREEXPORT	int		AesDecode(void* ctx, const BYTE* src, BYTE* dest, int length);

class AES: public Object
{
	DECLARE_REF_CLASS(AES)

private:
	void*	m_ctx;

public:
	AES(): m_ctx(NULL) {}
	~AES() { AesDeleteCtx(m_ctx); }

	inline	void	SetKey(const BYTE* key, int cbKey) { m_ctx = AesCreateCtx(key, cbKey); }
	inline	void	SetKey(ref<ByteArray> key);

	inline	int		Encode(const BYTE* src, BYTE* dest, int length)
	{
		return AesEncode(m_ctx, src, dest, length);
	}
	inline	int		Encode(ref<ByteArray> src, int offsrc, ref<ByteArray> dest, int offdest, int length);

	inline	int		Decode(const BYTE* src, BYTE* dest, int length)
	{
		return AesDecode(m_ctx, src, dest, length);
	}
	inline	int		Decode(ref<ByteArray> src, int offsrc, ref<ByteArray> dest, int offdest, int length);
};

// -------------------------------------------------------------------------- //

inline void AES::SetKey(ref<ByteArray> key)
{
	pin_ptr<BYTE> pinKey = key->GetBuffer();
	SetKey(pinKey.ptr(), key->Count());
}

inline int AES::Encode(ref<ByteArray> src, int offsrc, ref<ByteArray> dest, int offdest, int length)
{
	int srclen = src->Count(), destlen = dest->Count();
	if (length < 0 || offsrc > srclen || offsrc + length > srclen || offdest > destlen || offdest + length > destlen)
		cdec_throw(Exception(EC_OutOfRange));
	pin_ptr<BYTE> pinSrc = src->GetBuffer(), pinDest = dest->GetBuffer();
	return AesEncode(m_ctx, pinSrc.ptr() + offsrc, pinDest.ptr() + offdest, length);
}

inline int AES::Decode(ref<ByteArray> src, int offsrc, ref<ByteArray> dest, int offdest, int length)
{
	int srclen = src->Count(), destlen = dest->Count();
	if (length < 0 || offsrc > srclen || offsrc + length > srclen || offdest > destlen || offdest + length > destlen)
		cdec_throw(Exception(EC_OutOfRange));
	pin_ptr<BYTE> pinSrc = src->GetBuffer(), pinDest = dest->GetBuffer();
	return AesDecode(m_ctx, pinSrc.ptr() + offsrc, pinDest.ptr() + offdest, length);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
