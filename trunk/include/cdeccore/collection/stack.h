#pragma once

CDEC_NS_BEGIN
//---------------------------------------------------------------------------//

template<typename _Ty>
class StackV: public ArrayListV<_Ty>
{
	DECLARE_REF_CLASS(StackV<_Ty>)

public:
	typedef _Ty								value_type;
	typedef ArrayListV<_Ty>					base_type;
	typedef typename base_type::gc_type		gc_type;
	typedef typename base_type::trait_type	trait_type;
	typedef typename base_type::impl_type	impl_type;

protected:
	using base_type::m_vecBuffer;
	using base_type::Last;

public:
	void Push(value_type value)
	{
		Add(value);
	}

	value_type Pop()
	{
		value_type value = m_vecBuffer.back();
		m_vecBuffer.pop_back();
		return value;
	}

	value_type& Peek()
	{
		return Last();
	}
};



template<class _Ty>
class Stack: public StackV<ref<_Ty> >
{
	DECLARE_REF_CLASS(StackV<_Ty>)
};

//---------------------------------------------------------------------------//
CDEC_NS_END
