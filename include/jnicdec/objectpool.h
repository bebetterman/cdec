#pragma once

// -------------------------------------------------------------------------- //
// Object Pool
//
// For Java doesn't support a platform dependent integer type (e.g. 32-bit on
// x86 and 64-bit on x64), we have to keep the pointer in the C++ side.
// -------------------------------------------------------------------------- //

template<class _T>
class __ObjectPool
{
	static const int _capacity = 1024;
	_T _pool[_capacity];
	int _seek, _count;		// _pool[0] reserved for error
	ref<CriticalSection> m_cs;

public:
	__ObjectPool(): _seek(1), _count(1) { m_cs = gc_new<CriticalSection>(); }

	int Register(_T value)
	{
		m_cs->Enter();

		ASSERT(_count < _capacity);
		if (_count >= _capacity)
		{
			m_cs->Exit();
			throw std::bad_alloc();
		}

		for (; _seek < _capacity; ++_seek)
		{
			if (_pool[_seek] == NULL)
			{
				++_count;
				_pool[_seek] = value;
				int pos = _seek ++;
				m_cs->Exit();
				return pos;
			}
		}

		m_cs->Exit();
		return 0;
	}

	void Unregister(int index)
	{
		m_cs->Enter();
		ASSERT(_pool[index] != NULL && _count > 0);
		_pool[index] = NULL;
		--_count;
		_seek = index < _seek ? index : _seek;
		m_cs->Exit();
	}

	_T& operator[](int index) { return _pool[index]; }
};

inline __ObjectPool<void*>& PointerPool()
{
	static __ObjectPool<void*> pool;	// Singleton
	return pool;
}

template<class _T>
struct PooledPointer
{
	_T* _ptr;

	PooledPointer(int i) { _ptr = static_cast<_T*>(PointerPool()[i]); }

	_T* operator->() { return _ptr; }
	_T& operator*() { return *_ptr;	}
	operator _T*() { return _ptr; }

	static int Register(_T* p) { return PointerPool().Register(p); }

	static void Delete(int i)
	{
		delete static_cast<_T*>(PointerPool[i]);
		PointerPool().Unregister(i);
	}
};

inline __ObjectPool< ref<Object> >& ObjectPool()
{
	static __ObjectPool< ref<Object> > pool;	// Singleton
	return pool;
}

template<class _T>
struct PooledObject
{
	weak_ref<_T> _obj;

	PooledObject(int i) { _obj = ref_cast<_T>(ObjectPool()[i]); }

	_T* operator->() { return _obj.__GetPointer(); }
	_T& operator*() { return *_obj;	}
	operator _T*() { return _obj.__GetPointer(); }

	static int Register(ref<_T> obj) { return ObjectPool().Register(obj); }

	static void Delete(int i) { ObjectPool().Unregister(i); }
};

// -------------------------------------------------------------------------- //
