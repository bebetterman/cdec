// -------------------------------------------------------------------------- //
//	文件名		：	vector_provider.h
//	创建者		：	杨钢
//	创建时间	：	2007-12-21 16:55:21
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#pragma once

CDEC_EGG_NS_BEGIN

// -------------------------------------------------------------------------- //

// 注意：只能用于数值和指针类型，因为不会有任何构造析构函数被调用
template<class _Ty>
class VectorProvider
{
public:
	typedef VectorProvider<_Ty>		this_type;
	typedef _Ty						value_type;
	typedef size_t					size_type;
	typedef _Ty*					pointer;
	typedef const _Ty*				const_pointer;
	typedef _Ty&					reference;
	typedef const _Ty&				const_reference;

	// 没有调试能力
	typedef pointer					iterator;
	typedef const_pointer			const_iterator;

private:
	// m_buf 并非指向实际的分配地址，它是分配地址 + 8byte
	// size 和 capacity 存在这之前的 8 个字节中，类似 BSTR
	// 这样做是为了处理起来简单
	pointer		m_buf;

public:
	VectorProvider(): m_buf(NULL)
	{
		// x86 conflict
		ASSERT(sizeof(_Ty) == sizeof(DWORD));	
	}

	VectorProvider(const_pointer _F, const_pointer _L): m_buf(NULL)
	{
		assign(_F, _L);
	}

	~VectorProvider()
	{
		clear();
	}

	void assign(const_pointer _F, const_pointer _L)
	{
		ASSERT(_F != NULL && _L >= _F);
		ASSERT(m_buf == NULL || (m_buf + capacity()) <= _F || m_buf >= _L);

		size_type s = _L - _F;
		_assure(s);
		ASSERT(capacity() >= s);

		if (m_buf != NULL)
		{
			_size() = s;
			memcpy(m_buf, _F, s * sizeof(value_type));
		}
		else
		{
			ASSERT(s == 0);
		}
	}

	void clear()
	{
		if (m_buf != NULL)
		{
			_destroy_buffer(m_buf);
			m_buf = NULL;
		}
	}

	size_type capacity() const
	{
		return m_buf != NULL ? _capacity() : 0;
	}

	size_type size() const
	{
		return m_buf != NULL ? _size() : 0;
	}

	bool empty() const
	{
		return size() == 0;
	}

	reference at(size_type i)
	{
		ASSERT(i >= 0 && i < size());
		return m_buf[i];
	}

	const_reference at(size_type i) const
	{
		ASSERT(i >= 0 && i < size());
		return m_buf[i];
	}

	reference operator[] (size_type i)
	{
		return at(i);
	}

	const_reference operator[] (size_type i) const
	{
		return at(i);
	}
	
	iterator begin()
	{
		return m_buf;
	}

	const_iterator begin() const
	{
		return m_buf;
	}
	
	iterator end()
	{
		return m_buf + size();
	}

	const_iterator end() const
	{
		return m_buf + size();
	}
	
	reference front()
	{
		ASSERT(!empty());
		return *m_buf;
	}

	const_reference front() const
	{
		ASSERT(!empty());
		return *m_buf;
	}

	reference back()
	{
		ASSERT(!empty());
		return m_buf[size() - 1];
	}
	
	const_reference back() const
	{
		ASSERT(!empty());
		return m_buf[size() - 1];
	}

	void push_back(_Ty val)
	{
		DWORD	s = size();	// buf 可能为 NULL
		_assure(s + 1);
		m_buf[s] = val;	
		++_size();	// 这个一定要在 _assure 之后，否则复制数据绘越界
	}
	
	void pop_back()
	{
		ASSERT(size() > 0);
		--_size();
	}
	
	void _insert(size_type i, value_type val)
	{
		DWORD	s = size();
		_assure(s + 1);

		ASSERT(i >= 0 && i <= s);
		if (i < s)
			memmove(m_buf + i + 1, m_buf + i, (s - i) * sizeof(value_type));

		m_buf[i] = val;
		++_size();
	}

	void insert(iterator it, value_type val)
	{
		_insert(it - begin(), val);
	}

	void _erase(size_type i)
	{
		ASSERT(!empty());
		ASSERT(i >= 0 && i <= _size());
		if (i >= _size())
			return;

		size_type	s = --_size();
		if (i < s)
			memmove(m_buf + i, m_buf + i + 1, (s - i) * sizeof(value_type));
	}

	void _erase(size_type b, size_type e)
	{
		ASSERT(!empty());
		ASSERT(0 <= b && b <= e && e <= _size());
		if (e <= b)
			return;

		size_type s = _size() -= e - b;
		if (b < s)
			memmove(m_buf + b, m_buf + e, (s - b) * sizeof(value_type));
	}

	iterator erase(iterator it)
	{
		int		i = it - begin();
		_erase(i);
		ASSERT(begin() + i == it);	// 不一定，以后可能会有释放内存的动作
		return begin() + i;
	}

	iterator erase(iterator b, iterator e)
	{
		int		ib = b - begin(), ie = e - begin();
		_erase(ib, ie);
		ASSERT(begin() + ib == b);	// 不一定，以后可能会有释放内存的动作
		return begin() + ib;
	}

	void reserve(size_type n)
	{
		_assure(n);
	}

private:
	const DWORD& _capacity() const
	{
		return ((const DWORD*)m_buf)[-2];
	}

	DWORD& _capacity()
	{
		return ((DWORD*)m_buf)[-2];
	}

	const DWORD& _size() const
	{
		return ((const DWORD*)m_buf)[-1];
	}

	DWORD& _size()
	{
		return ((DWORD*)m_buf)[-1];
	}

	pointer _construct_buffer(size_type s)
	{
		UINT fullSize = s * sizeof(value_type) + sizeof(DWORD) * 2;
		void* p = CoreAllocT(fullSize);
		*(DWORD*)p = s;		// 更新了 Capacity，但是其他都没有初始化
		return (pointer)((BYTE*)p + sizeof(DWORD) * 2);
	}

	void _destroy_buffer(pointer p)
	{
		DWORD*	p2 = (DWORD*)p - 2;
		// free(p2);

		UINT fullSize = *p2 * sizeof(value_type) + sizeof(DWORD) * 2;
		CoreFreeT(p2, fullSize);
	}

	void _assure(size_type s)
	{
		if (s <= capacity())	// buf 可能为 NULL
			return;

		size_type	sRsv = GetMinPow2GE(s);
		ASSERT(sRsv >= s);
		pointer		bufNew = _construct_buffer(sRsv);

		if (m_buf != NULL)
		{
			// 更新 size
			DWORD size = _size();
			((DWORD*)bufNew)[-1] = size;
			// 复制原有 Buffer 到新值
			memcpy(bufNew, m_buf, size * sizeof(value_type));
			// 销毁旧的 Buffer
			_destroy_buffer(m_buf);
		}
		else
		{
			// 更新 size
			((DWORD*)bufNew)[-1] = 0;
		}

		m_buf = bufNew;
		ASSERT(_capacity() >= s);
	}
};

// -------------------------------------------------------------------------- //
// 单元测试部分

#if 0

void test1()
{
	int i;
	VectorProvider<int>		vec;
	ASSERT(vec.size() == 0 && vec.empty());
	ASSERT(vec.capacity() == 0);
	
	// push_back
	for (i = 1; i <= 5; ++i)
		vec.push_back(i * 2);
	ASSERT(vec.size() == 5 && !vec.empty());
	ASSERT(vec[0] == 2 && vec[1] == 4 && vec[2] == 6 && vec[3] == 8 && vec[4] == 10);
	ASSERT(vec.capacity() == 8);
	
	// front / back
	ASSERT(vec.front() == 2 && vec.back() == 10);
	
	// iterator
	VectorProvider<int>::iterator it;
	for (it = vec.begin(); it != vec.end(); ++it)
		*it += 1;
	for (it = vec.begin(); it != vec.end(); ++it)
		printf("%ld ", *it);
	printf("\n");
	
	// at
	vec[2] = 8;
	ASSERT(vec[2] == 8 && vec.size() == 5);
	
	// pop_back
	for (i = 1; i <= 5; ++i)
		vec.pop_back();
	ASSERT(vec.size() == 0 && vec.empty());
	ASSERT(vec.capacity() == 8);
	
	// empty-state
	ASSERT(vec.begin() == vec.end());
	
	// null-state
	vec.clear();
	ASSERT(vec.size() == 0 && vec.capacity() == 0 && vec.empty());
	ASSERT(vec.begin() == vec.end());
}

void test2()
{
	// assign
	int		i;
	const int INIT_LIST[] = { 1, 3, 5 };
	VectorProvider<int>		vec(INIT_LIST, INIT_LIST + countof(INIT_LIST));
	ASSERT(vec.size() == 3 && vec.front() == 1 && vec.back() == 5);
	
	vec.assign(INIT_LIST, INIT_LIST);
	ASSERT(vec.empty() && vec.capacity() == 4);
	
	// insert, erase (1)
	vec.assign(INIT_LIST, INIT_LIST + countof(INIT_LIST));
	vec.insert(0, 0);
	ASSERT(vec.size() == 4 && vec.front() == 0 && vec[1] == 1 && vec.back() == 5);
	
	vec.erase(0);
	ASSERT(vec.size() == 3 && vec.front() == 1 && vec.back() == 5);
	
	vec.insert(1, 2);
	ASSERT(vec.size() == 4 && vec.front() == 1 && vec[1] == 2 && vec[2] == 3 && vec.back() == 5);
	
	vec.erase(1);
	ASSERT(vec.size() == 3 && vec[0] == 1 && vec[1] == 3 && vec.back() == 5);
	
	vec.insert(2, 4);
	ASSERT(vec.size() == 4 && vec[0] == 1 && vec[1] == 3 && vec[2] == 4 && vec[3] == 5);
	
	vec.erase(2);
	ASSERT(vec.size() == 3 && vec[0] == 1 && vec[1] == 3 && vec.back() == 5);
	
	vec.insert(3, 6);
	ASSERT(vec.size() == 4 && vec[0] == 1 && vec[1] == 3 && vec[2] == 5 && vec[3] == 6);
	
	vec.erase(3);
	ASSERT(vec.size() == 3 && vec[0] == 1 && vec[1] == 3 && vec.back() == 5);
	
	for (i = 0; i < 3; ++i)
		vec.erase(0);
	ASSERT(vec.empty() && vec.capacity() > 0);
}

#endif
// -------------------------------------------------------------------------- //

CDEC_EGG_NS_END
