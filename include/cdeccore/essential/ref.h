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
	// 默认构造
	weak_ref(): _ptr(NULL) {}
	// 用于传递 this 指针或 NULL
	weak_ref(_Ty* ptr): _ptr(ptr) {}
	// 隐式类型转换
	template<class _U>
	weak_ref(const weak_ref<_U>& r): _ptr(r.__GetPointer()) {}
	// 复制构造必须在上一个方法后面
	weak_ref(const weak_ref<_Ty>& r): _ptr(r._ptr) {}
	// Only NULL is expected
	// weak_ref(int nil): _ptr(NULL) { ASSERT(nil == 0); }
	// 析构函数
	~weak_ref() { _ptr = NULL; }

	// 引用比较函数
	bool operator==(const weak_ref<_Ty>& r) const { return _ptr == r._ptr; }
	bool operator!=(const weak_ref<_Ty>& r) const {	return _ptr != r._ptr; }
	// 引用比较函数（指针只期望是 NULL）
	bool operator==(_Ty* _null) const { ASSERT(_null == 0); return _ptr == _null; };
	bool operator!=(_Ty* _null) const { ASSERT(_null == 0); return _ptr != _null; };

	// 赋值操作符
	template<class _U>
	weak_ref<_Ty>& operator=(const weak_ref<_U>& r) { _ptr = r.__GetPointer(); return *this; }
	weak_ref<_Ty>& operator=(const weak_ref<_Ty>& r) { _ptr = r._ptr; return *this; }
	weak_ref<_Ty>& operator=(_Ty* _null) { ASSERT(_null == NULL); _ptr = _null; return *this; }

	// 解引用
	_Ty* operator->() const { ASSERT(_ptr != NULL); return _ptr; }
	void operator*() const  { }

	// 内部使用，请不要使用此方法
	_Ty* __GetPointer() const { return _ptr; }
	// 禁止取地址
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
	// 默认构造
	ref() {}
	// 用于传递 this 指针或 NULL
	ref(_Ty* ptr) { _assign(ptr); }
	// 隐式类型转换
	template<class _U>
	ref(const weak_ref<_U>& r) { _assign(r.__GetPointer()); }
	// 复制构造必须在上一个方法后面
	ref(const ref<_Ty>& r) { _assign(r._ptr); }
	// Only NULL is expected
	// ref(int nil): weak_ref<_Ty>(nil) { }
	// 析构函数
	~ref() { _clear(); }

	// 赋值操作符
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
