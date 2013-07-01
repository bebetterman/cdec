// This header contains the following:
//   - Object
//   - DECLARE_REF_CLASS
//   - weak_ref / ref
//   - ref_cast

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

class Object
{
	virtual void	AddRef() = 0;
	virtual void	Release() = 0;
    template<typename _Ty> friend class ref;
};

// -------------------------------------------------------------------------- //

#define DECLARE_REF_CLASS(type)			\
public:									\
    virtual void	AddRef() = 0;		\
	virtual void	Release() = 0;

// -------------------------------------------------------------------------- //

template<class _Ty>
class weak_ref
{
protected:
	_Ty*	_ptr;

public:
	// Ĭ�Ϲ���
	weak_ref(): _ptr(NULL) {}
	// ���ڴ��� this ָ��� NULL
	weak_ref(_Ty* ptr): _ptr(ptr) {}
	// ��ʽ����ת��
	template<class _U>
	weak_ref(const weak_ref<_U>& r): _ptr(r.__GetPointer()) {}
	// ���ƹ����������һ����������
	weak_ref(const weak_ref<_Ty>& r): _ptr(r._ptr) {}
	// Only NULL is expected
	// weak_ref(int nil): _ptr(NULL) { ASSERT(nil == 0); }
	// ��������
	~weak_ref() { _ptr = NULL; }

	// ���ñȽϺ���
	bool operator==(const weak_ref<_Ty>& r) const { return _ptr == r._ptr; }
	bool operator!=(const weak_ref<_Ty>& r) const {	return _ptr != r._ptr; }
	// ���ñȽϺ�����ָ��ֻ������ NULL��
	bool operator==(_Ty* _null) const { ASSERT(_null == 0); return _ptr == _null; };
	bool operator!=(_Ty* _null) const { ASSERT(_null == 0); return _ptr != _null; };

	// ��ֵ������
	template<class _U>
	weak_ref<_Ty>& operator=(const weak_ref<_U>& r) { _ptr = r.__GetPointer(); return *this; }
	weak_ref<_Ty>& operator=(const weak_ref<_Ty>& r) { _ptr = r._ptr; return *this; }
	weak_ref<_Ty>& operator=(_Ty* _null) { ASSERT(_null == NULL); _ptr = _null; return *this; }

	// ������
	_Ty* operator->() const { ASSERT(_ptr != NULL); return _ptr; }
	void operator*() const  { }

	// �ڲ�ʹ�ã��벻Ҫʹ�ô˷���
	_Ty* __GetPointer() const { return _ptr; }
	// ��ֹȡ��ַ
	//void operator& () {}
};

// -------------------------------------------------------------------------- //

template<class _Ty>
class ref: public weak_ref<_Ty>
{
protected:
	using weak_ref<_Ty>::_ptr;		// gcc compliant

	void _assign(_Ty* p)
	{
		if (p != NULL)
			p->AddRef();
		if (_ptr != NULL)
			_ptr->Release();
		_ptr = p;
	}
	void _clear()
	{
		if (_ptr != NULL)
		{
			_ptr->Release();
			_ptr = NULL;
		}
	}

public:
	// Ĭ�Ϲ���
	ref() {}
	// ���ڴ��� this ָ��� NULL
	ref(_Ty* ptr) { _assign(ptr); }
	// ��ʽ����ת��
	template<class _U>
	ref(const weak_ref<_U>& r) { _assign(r.__GetPointer()); }
	// ���ƹ����������һ����������
	ref(const ref<_Ty>& r) { _assign(r._ptr); }
	// Only NULL is expected
	// ref(int nil): weak_ref<_Ty>(nil) { }
	// ��������
	~ref() { _clear(); }

	// ��ֵ������
	template<class _U>
	ref<_Ty>& operator=(const weak_ref<_U>& r) { _assign(r.__GetPointer()); return *this; }
	ref<_Ty>& operator=(const ref<_Ty>& r) { _assign(r._ptr); return *this;	}
	ref<_Ty>& operator=(_Ty* _null)	{ ASSERT(_null == NULL); _assign(_null); return *this; }
};

// -------------------------------------------------------------------------- //
template <typename _Ty>
struct _TypeTraits
{
    typedef _Ty		type;
};
template <typename _Ty>
struct _TypeTraits<ref<_Ty> >
{
    typedef typename _TypeTraits<_Ty>::type type;
};

// -------------------------------------------------------------------------- //

template<class _Ty>
class ref_cast: public ref<typename _TypeTraits<_Ty>::type>
{
public:
	template<class _U>
	ref_cast(const weak_ref<_U>& r)
	{
		_assign(static_cast<_Ty*>(r.__GetPointer()));
	}
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
