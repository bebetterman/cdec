// This header contains the following:
//   - foreach

#pragma once

CDEC_NS_BEGIN

template<typename T>
interface IEnumerator: Object
{
	DECLARE_REF_CLASS(IEnumerator<T >)

	virtual T		Current() = 0;
	virtual bool	Reset() = 0;
	virtual bool	Valid() = 0;
	virtual bool	Next() = 0;
};

template<typename T>
struct IEnumerable: Object
{
	DECLARE_REF_CLASS(IEnumerable<T >)

	typedef IEnumerator<T > enum_type;

	virtual ref<enum_type> GetEnumerator() = 0;
};

template<typename T>
struct Enumerable: IEnumerable<T>
{
	DECLARE_REF_CLASS(Enumerable<T >)

	typedef typename IEnumerable<T>::enum_type enum_type;

	ref<enum_type>	 _e;

	explicit Enumerable(ref<enum_type> e): _e(e) {}
	Enumerable(ref<IEnumerable<T > > lhs): _e(lhs->_e) {}

	ref<enum_type> GetEnumerator() { return _e; }
};

template<typename T>
struct _RunOnceProxy: ref<T>
{
	bool	m_f;
	_RunOnceProxy(ref<T> r): ref<T>(r), m_f(true) {}
	bool	isActive() { return m_f; }
	void	mute() { m_f = false; }
};

template<typename T>
struct _EnumTrait: _RunOnceProxy<T>
{
    typedef _RunOnceProxy<T> _Base;

	template<typename U>
	_EnumTrait(weak_ref<U> r): _Base::_RunOnceProxy(r->GetEnumerator()) {}

	template<typename U>
	_EnumTrait(ref<U> r): _Base::_RunOnceProxy(r->GetEnumerator()) {}

	template<typename U>
	_EnumTrait(gc_new<U> r): _Base::_RunOnceProxy(r->GetEnumerator()) {}

	//_EnumTrait(class stringx s): _Base::_RunOnceProxy(s.GetEnumerator()) {}
	template<typename U>
	_EnumTrait(U r): _Base::_RunOnceProxy(r.GetEnumerator()) {}
};

// CAUTION
// Avoid using names in any of these _Ty, _It, _Cl, _Et, for that will result undefined behavior in GCC
// Avoid using type reference such as MyCollection::item as type _Ty
// All above are probably caused by MACRO expanding machanism
#define foreach(_Ty, _It, _Cl)													\
	for (_EnumTrait<IEnumerator<_Ty > > _Et(_Cl); _Et.isActive(); _Et.mute())	\
	for (_Ty _It = _Et->Reset() ? _Et->Current() : _Ty(); _Et->Valid(); _It = _Et->Next() ? _Et->Current() : _Ty())

// -------------------------------------------------------------------------- //

template<typename T>
class _ContinuousEnum: public IEnumerator<T>
{
	DECLARE_REF_CLASS(_ContinuousEnum<T>)

protected:
	const T* m_p;
	int	m_i, m_n;

	_ContinuousEnum(): m_p(NULL), m_i(0), m_n(0) {}
	_ContinuousEnum(const T* p, int n): m_p(p), m_n(n), m_i(0) {}

public:
	T		Current() { return m_p[m_i]; }
	bool	Reset() { m_i = 0; return Valid(); }
	bool	Valid() { return m_n > m_i; }
	bool	Next() { ++m_i; return Valid(); }
};

template<typename It>
struct _IteratorEnum: public IEnumerator<It>
{
	DECLARE_REF_CLASS(_IteratorEnum<T>)

	It _it, _b, _e;

	_IteratorEnum(It b, It e): _b(b), _e(e) { Reset(); }

	template<class PU> _IteratorEnum(PU pu): _b(pu->begin()), _e(pu->end()) { Reset(); }
	It		Current() {	return _it;	}
	bool	Reset() { _it = _b; return Valid();	}
	bool	Valid() { return _it != _e;	}
	bool	Next() { ++_it; return Valid();	}
};

template<typename Value, typename Trait>
struct _IteratorTraitEnum: public IEnumerator<Value>
{
	DECLARE_REF_CLASS(_IteratorTraitEnum)

	typedef typename Trait::iterator_type It;
	It _it, _b, _e;

	_IteratorTraitEnum(It b, It e): _b(b), _e(e) { Reset(); }
	template<class PU> _IteratorTraitEnum(PU pu): _b(pu->begin()), _e(pu->end()) { Reset(); }

	Value	Current() {	return Trait::Get(_it);	}
	bool	Reset() { _it = _b; return Valid();	}
	bool	Valid() { return _it != _e;	}
	bool	Next() { ++_it; return Valid();	}
};

template<typename It> struct TraitIteratorValue
{
	typedef It iterator_type;
	typedef typename It::value_type value_type;
	static value_type Get(iterator_type it) { return *it; }
};

// -------------------------------------------------------------------------- //

#define foreach_it(T, it, b, e)	for (T it = (b); it != (e); ++it)

// -------------------------------------------------------------------------- //

CDEC_NS_END
