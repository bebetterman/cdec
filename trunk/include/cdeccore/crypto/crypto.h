#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

enum
{
	// HRESULT related
	EC_CRYPT_ST = MAKE_EX_CODE(ECNS_CRYPT, 0),
	EC_CRYPT_InvalidKeySize,
	EC_CRYPT_InvalidIVSize,
	EC_CRYPT_InvalidCipherMode,
	EC_CRYPT_DataNotAligned,
};

struct CryptoException: Exception
{
	CryptoException(int code): Exception(code) {}
};

// -------------------------------------------------------------------------- //

enum CipherMode
{
	Cipher_ECB,
	Cipher_CBC
};

// -------------------------------------------------------------------------- //

interface ICryptoTransform: public Object
{
	DECLARE_REF_CLASS(ICryptoTransform)

	virtual	void	Transform(const BYTE* src, BYTE* dest, int length) = 0;
	inline	void	Transform(ref<ByteArray> src, int offsrc, ref<ByteArray> dest, int offdest, int length);
};

// -------------------------------------------------------------------------- //

class CDECCOREEXPORT AES: public Object
{
	DECLARE_REF_CLASS(AES)

private:
	ref<ByteArray>	m_key, m_iv;
	CipherMode		m_mode;		// CipherMode

public:
	AES(): m_mode(Cipher_ECB) {}

	ref<ByteArray>	GetKey() { return m_key; }
	void	SetKey(ref<ByteArray> key);
	void	SetKey(const BYTE* key, int size);

	ref<ByteArray>	GetIV() { return m_iv; }
	void	SetIV(ref<ByteArray> iv);
	void	SetIV(const BYTE* iv, int size);

	CipherMode	GetMode() { return m_mode; }
	void	SetMode(CipherMode mode);

	ref<ICryptoTransform>	CreateEncryptor();
	ref<ICryptoTransform>	CreateDecryptor();
};

// -------------------------------------------------------------------------- //

inline void ICryptoTransform::Transform(ref<ByteArray> src, int offsrc, ref<ByteArray> dest, int offdest, int length)
{
	int srclen = src->Count(), destlen = dest->Count();
	if (length < 0 || offsrc > srclen || offsrc + length > srclen || offdest > destlen || offdest + length > destlen)
		cdec_throw(Exception(EC_OutOfRange));
	pin_ptr<BYTE> pinSrc = src->GetBuffer(), pinDest = dest->GetBuffer();
	Transform(pinSrc.ptr() + offsrc, pinDest.ptr() + offdest, length);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
