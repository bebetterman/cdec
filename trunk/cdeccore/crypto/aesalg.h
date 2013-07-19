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

class AesBaseTransform: public ICryptoTransform
{
	DECLARE_REF_CLASS(AesBaseTransform)

protected:
	CAesAlg*			m_alg;
	CipherMode	m_mode;

public:
	AesBaseTransform(CAesAlg* alg, CipherMode mode);
	~AesBaseTransform();
};

class AesEncryptorTransform: public AesBaseTransform
{
	DECLARE_REF_CLASS(AesEncryptorTransform)

public:
	AesEncryptorTransform(CAesAlg* alg, CipherMode mode): AesBaseTransform(alg, mode) {}
	void Transform(const BYTE* src, BYTE* dest, int length);
};

class AesDecryptorTransform: public AesBaseTransform
{
	DECLARE_REF_CLASS(AesDecryptorTransform)

public:
	AesDecryptorTransform(CAesAlg* alg, CipherMode mode): AesBaseTransform(alg, mode) {}
	void Transform(const BYTE* src, BYTE* dest, int length);
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
