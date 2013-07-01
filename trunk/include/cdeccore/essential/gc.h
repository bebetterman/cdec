// This header contains the following:
//   - gc_new

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

template<class _Ty>
class GcCreation
{
public:
	// Allocate memory for a single instance
	inline static _Ty* AllocInstanceMemory()
	{
		return (_Ty*)CoreAllocT(sizeof(_Ty));
	}

	// Allocate memory for a fixed array
	inline static _Ty* AllocArrayMemory(size_t count)
	{
		return (_Ty*)CoreAllocT(sizeof(_Ty) * count);
	}

	// Free memory allocated for a single instance
	inline static void FreeInstanceMemory(_Ty* p)
	{
		CoreFreeT(p, sizeof(_Ty));
	}

	// Free memory allocated for a fixed array
	inline static void FreeArrayMemory(_Ty* p, size_t count)
	{
		CoreFreeT(p, sizeof(_Ty) * count);
	}

	// Create a single instance with default constructor
	inline static _Ty* CreateInstance()
	{
		_Ty* p = AllocInstanceMemory();
		new(p) _Ty();
		return p;
	}

	// Create a fixed array with default constructor
	inline static _Ty* CreateArray(size_t count)
	{
		_Ty* p = AllocArrayMemory(count);
		for (size_t i = 0; i < count; ++i)
			new(p + i) _Ty();
		return p;
	}

	// Destroy a single instance
	// A NULL pointer is accepted and simply ignored
	inline static void DestroyInstance(_Ty* p)
	{
		if (p != NULL)
		{
			p->~_Ty();
			FreeInstanceMemory(p);
		}
	}

	// Destroy a fixed array
	// If the pointer is not NULL, count is required to be greater than zero
	// If the pointer is NULL, count must be zero. At this case, the function simply
	// does nothing.
	inline static void DestroyArray(_Ty* p, size_t count)
	{
		if (p != NULL)
		{
			ASSERT(count > 0);
			for (size_t i = 0; i < count; ++i)
				p[i].~_Ty();
			FreeArrayMemory(p, count);
		}
		else
		{
			ASSERT(count == 0);
		}
	}
};

// -------------------------------------------------------------------------- //
// Reference Counter for internal classes
// -------------------------------------------------------------------------- //

#if defined(X_OS_WINDOWS)
#	define REFCOUNT_ADD(x)				_InterlockedIncrement((long*)&x)
#	define REFCOUNT_RELEASE_TEST(x)		(_InterlockedDecrement((long*)&x) == 0)
#elif defined(X_OS_LINUX)
#	define REFCOUNT_ADD(x)				__sync_add_and_fetch(&x,1)
#	define REFCOUNT_RELEASE_TEST(x)		(__sync_sub_and_fetch(&x,1) == 0)
#elif 0
#	define REFCOUNT_ADD(x)				(++x)
#	define REFCOUNT_RELEASE_TEST(x)		(--x == 0)
#endif

struct RefCounter
{
	int		_value;
	RefCounter(): _value(1) {}
	void	AddRef() { REFCOUNT_ADD(_value); }
	bool	ReleaseAndTest() { return REFCOUNT_RELEASE_TEST(_value); }
	operator UINT() const { return _value; }
};

// The following line works in VC but incompliant to gcc
// friend _gcCreation
//
#define _CDEC_SHARED_CLASS(_Ty)								\
	protected:												\
		typedef GcCreation<_Ty > _gcCreation;				\
		friend class GcCreation<_Ty >;						\
		RefCounter	_ref;									\
	public:													\
		void	AddRef() { _ref.AddRef(); }					\
		void	Release() {									\
			if (_ref.ReleaseAndTest()) {					\
				_gcCreation::DestroyInstance(this);			\
			}												\
		}

// -------------------------------------------------------------------------- //
// Object wrapper for general reference class
// -------------------------------------------------------------------------- //

template<class _Ty>
class ObjectWrapper: public _Ty
{
protected:
	RefCounter	_ref;
public:
	ObjectWrapper(): _Ty() {}
	template<class _Arg1>
	ObjectWrapper(_Arg1 arg1): _Ty(arg1) {}
	template<class _Arg1, class _Arg2>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2): _Ty(arg1, arg2) {}
	template<class _Arg1, class _Arg2, class _Arg3>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3): _Ty(arg1, arg2, arg3) {}
	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4): _Ty(arg1, arg2, arg3, arg4) {}
	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5): _Ty(arg1, arg2, arg3, arg4, arg5) {}
	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5, _Arg6 arg6): _Ty(arg1, arg2, arg3, arg4, arg5, arg6) {}

	void	AddRef() { return _ref.AddRef(); }
	void	Release()
	{
		if (_ref.ReleaseAndTest())
		{
			GcCreation<ObjectWrapper<_Ty> >::DestroyInstance(this);
		}
	}
};

template<class Ty>
class gc_new: public ref<typename _TypeTraits<Ty>::type>
{
    typedef typename _TypeTraits<Ty>::type _Ty;

    using weak_ref<_Ty>::_ptr;		// gcc compliant

public:
    typedef ObjectWrapper<_Ty> WrapperType;
    typedef GcCreation<WrapperType> GcCreationType;

	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5, _Arg6 arg6)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3, arg4, arg5, arg6);
	}

	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3, arg4, arg5);
	}

	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3, arg4);
	}

	template<class _Arg1, class _Arg2, class _Arg3>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3);
	}

	template<class _Arg1, class _Arg2>
	gc_new(_Arg1 arg1, _Arg2 arg2)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2);
	}

	template<class _Arg1>
	gc_new(_Arg1 arg1)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1);
	}

	gc_new()
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType();
	}
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
