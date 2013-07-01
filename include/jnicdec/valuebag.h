#pragma once

// -------------------------------------------------------------------------- //
// Value Bag
// -------------------------------------------------------------------------- //

template<class _T>
class ValueBag
{
public:
	typedef _T	value_type;

private:
	// a vector_p will provide higher performance
	UINT		m_len;
	_T			m_values[1];

private:
	ValueBag() {}
	~ValueBag() {}

	static UINT _Size(UINT len) { return len * sizeof(_T) + sizeof(UINT); }

public:
	static ValueBag<_T>* Create(UINT len);
	static ValueBag<_T>* Create(const _T* values, UINT len);

	static void Delete(ValueBag<_T>* p) { if (p != NULL) PoolFree(p, _Size(p->m_len)); }

	UINT Len() const { return m_len; }
	_T GetAt(UINT i) const { return m_values[i]; }
	const _T* Value() const { return m_values; }
	_T* Value() { return m_values; }

	UINT GetHash() const;
};

template<class _TKey, class _TValue>
struct Entry
{
	_TKey*		Key;
	_TValue*	Value;

	Entry(): Key(NULL), Value(NULL) {}
	Entry(_TKey* _key, _TValue* _value): Key(_key), Value(_value) {}

	void Delete() { _TKey::Delete(Key); _TValue::Delete(Value); }
};

// -------------------------------------------------------------------------- //

template<class _T>
ValueBag<_T>* ValueBag<_T>::Create(UINT len)
{
	ValueBag<_T>* p = (ValueBag<_T>*)PoolAlloc(_Size(len));
	p->m_len = len;
	return p;
}

template<class _T>
ValueBag<_T>* ValueBag<_T>::Create(const _T* values, UINT len)
{
	ValueBag<_T>* p = Create(len);
	memcpy(p->m_values, values, len * sizeof(_T));
	return p;
}

template<class _T>
UINT ValueBag<_T>::GetHash() const
{
	UINT hash = 0;
	const _T *pe = m_values + m_len;
	for (const _T *p = m_values; p < pe; ++p)
	{
		hash *= 16777619;
		hash ^= (UINT)*p;
	}
	return hash;
}

// -------------------------------------------------------------------------- //

typedef ValueBag<char>	StringBagA;
typedef ValueBag<WCHAR>	StringBagW;
typedef ValueBag<BYTE>	BinaryBag;

// -------------------------------------------------------------------------- //

inline StringBagW* CreateUnicodeStringBagFromJString(JNIEnv* env, jstring val)
{
	ASSERT(sizeof(jchar) == sizeof(WCHAR));
	int len = env->GetStringLength(val);
	const jchar* pval = env->GetStringChars(val, NULL);
	StringBagW* val_b = StringBagW::Create((const WCHAR*)pval, len);
	env->ReleaseStringChars(val, pval);

	return val_b;
}

inline StringBagA* CreateAnsiStringBagFromJString(JNIEnv* env, jstring val)
{
	const char* p = env->GetStringUTFChars(val, NULL);
	int len = env->GetStringUTFLength(val);
	StringBagA* val_b = StringBagA::Create(p, len);
	env->ReleaseStringUTFChars(val, p);

	return val_b;
}

inline BinaryBag* CreateBinaryBagFromJByteArray(JNIEnv* env, jbyteArray val)
{
	ASSERT(sizeof(jbyte) == sizeof(BYTE));
	int size = env->GetArrayLength(val);
	BinaryBag* val_b = BinaryBag::Create(size);
	env->GetByteArrayRegion(val, 0, size, (jbyte*)val_b->Value());

	return val_b;
}

inline jstring BuildJStringAnsi(JNIEnv* env, const char* buf, int len)
{
	ASSERT(sizeof(jchar) == sizeof(WCHAR));
	if (buf == NULL)
		return NULL;

	const int maxlen = 256;
	WCHAR m_wbuf[maxlen];
	if (len > maxlen)
	{
		ASSERT(false);	// Truncated
		len = maxlen;
	}
	for (int i = 0; i < len; ++i)
		m_wbuf[i] = buf[i];
	return env->NewString((const jchar*)m_wbuf, len);
}

inline jstring BuildJStringAnsi(JNIEnv* env, const StringBagA* val)
{
	return val != NULL ? BuildJStringAnsi(env, val->Value(), val->Len()) : NULL;
}

inline void WriteJByteArray(JNIEnv* env, jbyteArray arr, int offset, int count, const BYTE* buf)
{
	ASSERT(sizeof(jbyte) == sizeof(BYTE));
	ASSERT(arr != NULL && offset >= 0 && count >= 0 && offset + count <= env->GetArrayLength(arr) && buf != NULL);
	env->SetByteArrayRegion(arr, offset, count, (jbyte*)buf);
}

inline jbyteArray BuildJByteArray(JNIEnv* env, const BYTE* buf, int size)
{
	if (buf == NULL)
		return NULL;
	
	jbyteArray arr = env->NewByteArray(size);
	WriteJByteArray(env, arr, 0, size, buf);
	return arr;
}

inline jbyteArray BuildJByteArray(JNIEnv* env, const BinaryBag* value)
{
	if (value == NULL)
		return NULL;

	return BuildJByteArray(env, value->Value(), value->Len());
}

// -------------------------------------------------------------------------- //
// Auto free value bags
// For local use in JNI functions
// -------------------------------------------------------------------------- //

template<class _T>
struct AutoValueBag
{
	typedef ValueBag<_T> ValueBagT;
	ValueBagT*	m_val;

	AutoValueBag(): m_val(NULL) {}
	AutoValueBag(const _T* p, UINT len) { m_val = ValueBagT::Create(p, len); }
	AutoValueBag(ValueBagT* val): m_val(val) {}
	AutoValueBag(AutoValueBag& rhs)
	{
		// Ownership replacement
		ASSERT(this != &rhs);
		m_val = rhs.m_val;
		rhs.m_val = NULL;
	}
	~AutoValueBag() { ValueBagT::Delete(m_val); }
	
	operator const ValueBagT*() const { return m_val; }
	ValueBagT* detach()
	{
		// Ownership release
		ValueBagT* val = m_val;
		m_val = NULL;
		return val;
	}

	const _T* raw_data() const { return m_val != NULL ? m_val->Value() : NULL; }
	UINT size() const { return m_val != NULL ? m_val->Len() : 0; }
};

struct AutoAnsiStringBag: AutoValueBag<char>
{
	AutoAnsiStringBag(ValueBagT* val): AutoValueBag<char>(val) {}
	AutoAnsiStringBag(AutoValueBag<char>& rhs): AutoValueBag<char>(rhs) {}
	AutoAnsiStringBag(JNIEnv* env, jstring val) { m_val = CreateAnsiStringBagFromJString(env, val); }
};

struct AutoUnicodeStringBag: AutoValueBag<WCHAR>
{
	AutoUnicodeStringBag(ValueBagT* val): AutoValueBag<WCHAR>(val) {}
	AutoUnicodeStringBag(AutoValueBag<WCHAR>& rhs): AutoValueBag<WCHAR>(rhs) {}
	AutoUnicodeStringBag(JNIEnv* env, jstring val) { m_val = CreateUnicodeStringBagFromJString(env, val); }
	operator cdec::stringx() { return m_val != NULL ? cdec::stringx(raw_data(), size()) : cdec::stringx(); }
};

struct AutoBinaryBag: AutoValueBag<BYTE>
{
	AutoBinaryBag(ValueBagT* val): AutoValueBag<BYTE>(val) {}
	AutoBinaryBag(AutoValueBag<BYTE>& rhs): AutoValueBag<BYTE>(rhs) {}
	AutoBinaryBag(JNIEnv* env, jbyteArray val) { m_val = CreateBinaryBagFromJByteArray(env, val); }
};

// -------------------------------------------------------------------------- //
// Pointer
// -------------------------------------------------------------------------- //

template<class _T>
struct JniPtr
{
	_T*	_ptr;

	JniPtr(INT64 po) { _ptr = (_T*)(INT_PTR)po; }

	_T* operator->() { return _ptr; }
	_T& operator*() { return *_ptr;	}
	operator _T*() { return _ptr; }

	static INT64 Value(_T* ptr) { return (INT_PTR)ptr; }
	static void Delete(INT64 po) { delete (_T*)(INT_PTR)po; }
};

// -------------------------------------------------------------------------- //

struct JniByteArray
{
	JNIEnv*		_env;
	jbyteArray	_arr;
	jbyte*		_ptr;

	JniByteArray(JNIEnv* env, jbyteArray arr) { _env = env; _arr = arr; _ptr = env->GetByteArrayElements(arr, NULL); }
	~JniByteArray() { _env->ReleaseByteArrayElements(_arr, _ptr, 0); }

	static size_t size_of(JNIEnv* env, jbyteArray arr) { return env->GetArrayLength(arr); }
	size_t size() const { return size_of(_env, _arr); }

	operator BYTE*() { return (BYTE*)_ptr; }
	operator const BYTE*() const { return (const BYTE*)_ptr; }
};

// -------------------------------------------------------------------------- //
