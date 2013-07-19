// -------------------------------------------------------------------------- //
//	文件名		：	AesAlg.h
//	创建者		：	Zhang Fan
//	创建时间	：	2010-4-28 21:51:00
//	功能描述	：	Aes算法
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

typedef unsigned char Byte;
typedef unsigned int UInt32;
typedef unsigned int UINT;

const UINT AES_BLOCK_SIZE = 16;

typedef struct
{
	unsigned	numRounds2; /* = numRounds / 2 */
	UInt32		rkey[(14 + 1) * 4];
} AesCTX;

// -------------------------------------------------------------------------- //

class CAesAlg
{
private:
	Byte	m_Sbox[256];
	UInt32	m_T[256 * 4];
	UInt32	m_D[256 * 4];
	Byte	m_InvS[256];
	Byte	m_Rcon[11];

	AesCTX	m_AesEncodeCTX;
	AesCTX	m_AesDecodeCTX;

public:
	CAesAlg();
	void SetKey(const Byte *key, unsigned keySize);
	void Aes_Encode32(UInt32 *dest, const UInt32 *src);
	void Aes_Decode32(UInt32 *dest, const UInt32 *src);

private:
	void _AesGenTables(void);
	void _SetKeyEncode(AesCTX &aes, const Byte *key, unsigned keySize);
	void _SetKeyDecode(const Byte *key, unsigned keySize);
	void _AesEncode32(UInt32 *dest, const UInt32 *src, const UInt32 *w, unsigned numRounds2);
	void _AesDecode32(UInt32 *dest, const UInt32 *src, const UInt32 *w, unsigned numRounds2);
};

// -------------------------------------------------------------------------- //
// Cdec implementation
// -------------------------------------------------------------------------- //

class AesTransform: public ICryptoTransform
{
	DECLARE_REF_CLASS(AesTransform)

protected:
	CAesAlg*	m_alg;
	CipherMode	m_mode;
	BYTE		m_iv[16];

	typedef void (AesTransform::* f_Encoder)(const BYTE* src, BYTE* dest, int length);
	f_Encoder	m_e;

public:
	AesTransform(CAesAlg* alg, CipherMode mode, ref<ByteArray> iv, bool fEncoder);
	~AesTransform();

	void Transform(const BYTE* src, BYTE* dest, int length)
	{
		if ((length & 0xF) != 0)
			cdec_throw(CryptoException(EC_CRYPT_DataNotAligned));
		return (this->*m_e)(src, dest, length);
	}

protected:
	void	f_EncodeECB(const BYTE* src, BYTE* dest, int length);
	void	f_DecodeECB(const BYTE* src, BYTE* dest, int length);

	void	f_EncodeCBC(const BYTE* src, BYTE* dest, int length);
	void	f_DecodeCBC(const BYTE* src, BYTE* dest, int length);

	static void	XOR16(BYTE* dest, const BYTE* src);
	static void COPY16(BYTE* dest, const BYTE* src);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
