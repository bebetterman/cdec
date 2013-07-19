// This header contains the following:
//   - ArrayV
//	 - Array

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Array for Value Types
// -------------------------------------------------------------------------- //

template<class _Ty>
class ArrayV: public Object
{
	DECLARE_REF_CLASS(Array<_Ty>)

public:
	typedef _Ty					value_type;
	typedef GcCreation<_Ty>		gc_type;

protected:
	size_t			m_count;
	_Ty*			m_buffer;

protected:
	ArrayV(size_t count) : m_count(count), m_buffer(NULL)
	{
		if (m_count != 0)
		{
			m_buffer = gc_type::CreateArray(m_count);
			ASSERT(m_buffer != NULL);
		}
	}

	ArrayV(const _Ty* origin, size_t count): m_count(count), m_buffer(NULL)
	{
		if (m_count != 0)
		{
			m_buffer = gc_type::CreateArray(m_count);
			ASSERT(m_buffer != NULL);
		}

		for (size_t i = 0; i < count; ++i)
			m_buffer[i] = *origin++;
	}

	~ArrayV()
	{
		gc_type::DestroyArray(m_buffer, m_count);
	}

public:
	size_t	Count() const { return m_count; }

	_Ty&	at(size_t index) { ASSERT(index < m_count); return m_buffer[index]; }
	_Ty&	operator[] (size_t index) { return at(index); }

	pin_ptr<_Ty> GetBuffer() { return pin_ptr<_Ty>::CreatePinptr(m_buffer); }

	ref<IEnumerator<_Ty> > GetEnumerator()
	{
		return gc_new<_ContinuousEnum<_Ty> >(m_buffer, m_count);
	}

	void	Sort() { std::sort(m_buffer, m_buffer + m_count); }

	void	CopyFrom(int offT, ref<ArrayV<_Ty> > s, int offS, int count)
	{
		if (offT < 0 || offS < 0 || count < 0 || offT + count > Count() || offS + count > s->Count())
			cdec_throw(Exception(EC_OutOfRange));
		_Ty* pt = m_buffer + offT;
		_Ty* ps = s->m_buffer + offS;
		for (int i = 0; i < count; ++i)
			*pt++ = *ps++;
	}

	void	MemoryCopyFrom(int offT, const _Ty* ps, int count)
	{
		if (offT < 0 || count < 0 || offT + count > Count())
			cdec_throw(Exception(EC_OutOfRange));
		_Ty* pt = m_buffer + offT;
		memcpy(pt, ps, count * sizeof(_Ty));
	}

	void	MemoryCopyFrom(int offT, ref<ArrayV<_Ty> > s, int offS, int count)
	{
		if (offT < 0 || offS < 0 || count < 0 || offT + count > Count() || offS + count > s->Count())
			cdec_throw(Exception(EC_OutOfRange));
		_Ty* pt = m_buffer + offT;
		_Ty* ps = s->m_buffer + offS;
		memcpy(pt, ps, count * sizeof(_Ty));
	}
};

// -------------------------------------------------------------------------- //
// Array for Reference Types
// -------------------------------------------------------------------------- //

template<class _Ty>
class Array: public ArrayV<ref<_Ty> >
{
	DECLARE_REF_CLASS(Array<_Ty>)

public:
	typedef ArrayV<ref<_Ty> >	base_type;

protected:
	using base_type::m_buffer;

	Array(size_t count) : base_type(count) {}
	Array(const ref<_Ty>* origin, size_t count): base_type(origin, count) {}
};

// -------------------------------------------------------------------------- //
// FAQ Alias
// -------------------------------------------------------------------------- //

typedef ArrayV<BYTE>		ByteArray;
typedef ArrayV<WORD>		WordArray;
typedef ArrayV<DWORD>		DWordArray;

typedef ArrayV<INT8>		Int8Array;
typedef ArrayV<INT16>		Int16Array;
typedef ArrayV<INT32>		Int32Array;
typedef ArrayV<INT64>		Int64Array;

typedef ArrayV<UINT8>		UInt8Array;
typedef ArrayV<UINT16>		UInt16Array;
typedef ArrayV<UINT32>		UInt32Array;
typedef ArrayV<UINT64>		UInt64Array;

typedef ArrayV<int>			IntArray;
typedef ArrayV<UINT>		UIntArray;
typedef ArrayV<WCHAR>		CharArray;
typedef ArrayV<stringx>		StringArray;

// -------------------------------------------------------------------------- //
CDEC_NS_END
