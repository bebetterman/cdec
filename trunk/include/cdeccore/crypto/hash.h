#pragma once

namespace kfcimpl
{
	class MD5;
	class SHA1;
};

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

CDECCOREEXPORT ref<ByteArray> HashAlgorithmCompute(interface HashAlgorithm* pThis, ref<Stream> istream);

interface HashAlgorithm: Object
{
	DECLARE_REF_CLASS(HashAlgorithm)

	virtual int		_DigestSize() = 0;

	virtual void	Reset() = 0;
	virtual void	Update(const void* input, int size) = 0;
	virtual void	Final(BYTE* digest) = 0;

	virtual			~HashAlgorithm() { }

	inline void		Update(ref<ByteArray> input, int offset, int size);
	inline ref<ByteArray>	Final();

	inline ref<ByteArray>	Compute(const void* input, int size);
	inline ref<ByteArray>	Compute(ref<ByteArray> data, int offset, int size);
	inline ref<ByteArray>	Compute(ref<ByteArray> data) { return Compute(data, 0, data->Count()); }
	inline ref<ByteArray>	Compute(ref<Stream> istream) { return HashAlgorithmCompute(this, istream); }
};

class CDECCOREEXPORT MD5: public HashAlgorithm
{
protected:
	DECLARE_REF_CLASS(MD5)

	kfcimpl::MD5*	m_impl;

public:
	MD5();
	~MD5();

	static ref<MD5> Create() { return gc_new<MD5>(); }

	using HashAlgorithm::Update;
	using HashAlgorithm::Final;

	void	Reset();
	void	Update(const void* input, int size);
	void	Final(BYTE digest[16]);

	int		_DigestSize() { return 16; }
};

class CDECCOREEXPORT SHA1: public HashAlgorithm
{
protected:
	DECLARE_REF_CLASS(SHA1)

	kfcimpl::SHA1*	m_impl;

public:
	SHA1();
	~SHA1();

	static ref<SHA1> Create() { return gc_new<SHA1>(); }

	using HashAlgorithm::Update;
	using HashAlgorithm::Final;

	void	Reset();
	void	Update(const void* input, int size);
	void	Final(BYTE digest[20]);

	int		_DigestSize() { return 20; }
};

// -------------------------------------------------------------------------- //

inline void HashAlgorithm::Update(ref<ByteArray> input, int offset, int size)
{
	if (offset < 0 || size < 0 || offset + size > input->Count())
		cdec_throw(Exception(EC_OutOfRange));

	pin_ptr<BYTE> pinInput = input->GetBuffer();
	Update(pinInput.ptr() + offset, size);
}

inline ref<ByteArray> HashAlgorithm::Final()
{
	ref<ByteArray> digest = gc_new<ByteArray>(_DigestSize());
	pin_ptr<BYTE> pinOutput = digest->GetBuffer();
	Final(pinOutput.ptr());
	return digest;
}

inline ref<ByteArray> HashAlgorithm::Compute(const void* input, int size)
{
	Reset();
	Update(input, size);
	return Final();
}

inline ref<ByteArray> HashAlgorithm::Compute(ref<ByteArray> data, int offset, int size)
{
	Reset();
	Update(data, offset, size);
	return Final();
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
