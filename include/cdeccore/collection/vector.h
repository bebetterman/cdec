#pragma once

#if CDEC_USE_KFC_ENHANCE
//#include "../../service/alg_memfix.h"
//#include "../../rtl/colspec/colspc_vector_provider.h"
#endif

//---------------------------------------------------------------------------//
CDEC_NS_BEGIN

//---------------------------------------------------------------------------//

template<class _Ty>
struct ArrayListVTrait
{
	typedef std::vector<_Ty> ImplType;
};

#if CDEC_USE_KFC_ENHANCE

template<>
struct ArrayListVTrait<int>
{
	typedef kfc::VectorProvider<int> ImplType;
};

#endif

//---------------------------------------------------------------------------//
// ArrayList for Value Types
//---------------------------------------------------------------------------//

template<class _Ty>
class ArrayListV: public Object
{
	DECLARE_REF_CLASS(ArrayListV<_Ty>)

public:
	typedef _Ty								value_type;
	typedef GcCreation<value_type>			gc_type;
	typedef ArrayListVTrait<value_type>		trait_type;
	typedef typename trait_type::ImplType	impl_type;

protected:
	impl_type	m_vecBuffer;

protected:
	ArrayListV(){}
	ArrayListV(size_t count) { Reserve(count); }
	~ArrayListV(){}

public:
	void Add(value_type _value)
 	{
		m_vecBuffer.push_back(_value);
	}

	value_type& at(size_t pos)
	{
		return m_vecBuffer[pos];
	}

	value_type&	operator[] (size_t index)
	{
		return at(index);
	}

	void Insert(size_t pos, value_type _value)
	{
		ASSERT(pos >= 0 && pos <= m_vecBuffer.size());
		m_vecBuffer.insert(m_vecBuffer.begin() + pos, _value);
	}

	void RemoveRange(size_t first_Pos, size_t last_Pos)
	{
		ASSERT(first_Pos >= 0 && first_Pos <= m_vecBuffer.size()
				&& last_Pos >= 0 && last_Pos <= m_vecBuffer.size()
					&& first_Pos < last_Pos);
		m_vecBuffer.erase(m_vecBuffer.begin() + first_Pos, m_vecBuffer.begin() + last_Pos);
	}

	void RemoveAt(size_t pos)
	{
		ASSERT(pos >= 0 && pos <= m_vecBuffer.size());
		m_vecBuffer.erase(m_vecBuffer.begin() + pos);
	}

	size_t Count()
	{
		return m_vecBuffer.size();
	}

	size_t Capacity()
	{
		return m_vecBuffer.capacity();
	}

	void Reserve(size_t count)
	{
		m_vecBuffer.reserve(count);
	}

	void Resize(size_t count)
	{
		m_vecBuffer.resize(count);
	}

	void Resize(size_t count, _Ty value)
	{
		m_vecBuffer.resize(count, value);
	}

	void Clear()
	{
		m_vecBuffer.clear();
	}

	bool Empty()
	{
		return m_vecBuffer.empty();
	}

	void RemoveLast()
	{
		m_vecBuffer.pop_back();
	}

	value_type& First()
	{
		return m_vecBuffer.front();
	}

	value_type& Last()
	{
		return m_vecBuffer.back();
	}

	pin_ptr<_Ty> GetBuffer() { return pin_ptr<_Ty>::CreatePinptr(&m_vecBuffer[0]); }

	ref<IEnumerator<_Ty> > GetEnumerator()
	{
		_Ty* buf = !m_vecBuffer.empty() ? &m_vecBuffer[0] : NULL;
		return gc_new<_ContinuousEnum<_Ty> >(buf, Count());
	}

	void Sort() { std::sort(m_vecBuffer.begin(), m_vecBuffer.end()); }
};

//---------------------------------------------------------------------------//
// ArrayList for Reference Types
//---------------------------------------------------------------------------//

template<class _Ty>
class ArrayList: public ArrayListV<ref<_Ty> >
{
	DECLARE_REF_CLASS(ArrayListV<_Ty>)
};

// -------------------------------------------------------------------------- //
// FAQ Alias
// -------------------------------------------------------------------------- //

typedef ArrayListV<BYTE>		ByteArrayList;
typedef ArrayListV<WORD>		WordArrayList;
typedef ArrayListV<DWORD>		DWordArrayList;

typedef ArrayListV<INT8>		Int8ArrayList;
typedef ArrayListV<INT16>		Int16ArrayList;
typedef ArrayListV<INT32>		Int32ArrayList;
typedef ArrayListV<INT64>		Int64ArrayList;

typedef ArrayListV<UINT8>		UInt8ArrayList;
typedef ArrayListV<UINT16>		UInt16ArrayList;
typedef ArrayListV<UINT32>		UInt32ArrayList;
typedef ArrayListV<UINT64>		UInt64ArrayList;

typedef ArrayListV<int>			IntArrayList;
typedef ArrayListV<UINT>		UIntArrayList;
typedef ArrayListV<WCHAR>		CharArrayList;
typedef ArrayListV<stringx>		StringArrayList;

//---------------------------------------------------------------------------//
CDEC_NS_END
