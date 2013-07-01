#pragma once

#ifndef __KFC_JNI__

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

inline void* PoolAlloc(UINT cb) { return malloc(cb); }
inline void PoolFree(void* p, UINT) { free(p); }

#endif

// -------------------------------------------------------------------------- //

template<class _TKey, class _TValue>
struct MassSetEntry: Entry<_TKey, _TValue>
{
	UINT		Hash;

	MassSetEntry(): Hash(0) {}
	MassSetEntry(UINT _hash, _TKey* _key, _TValue* _value): Hash(_hash), Entry<_TKey, _TValue>(_key, _value) {}
};

template<class _TKey, class _TValue>
class MassSetRow
{
public:
	typedef MassSetEntry<_TKey, _TValue>	EntryT;

private:
#if USE_VECTORPROVIDER
	typedef VectorProvider<EntryT>	EntryVectorT;
#else
	typedef std::vector<EntryT>		EntryVectorT;
#endif
	EntryVectorT			m_row;

public:
	~MassSetRow() { Clear(); }

	bool	AddEntry(UINT hash, _TKey* key, _TValue* value);
	Entry<_TKey, _TValue>*	GetEntry(UINT hash, const _TKey* key);
	bool	RemoveEntry(UINT hash, const _TKey* key);
	void	Clear();

	UINT	Size() const { return (UINT)m_row.size(); }
	EntryT&	GetAt(UINT i) { return m_row[i]; }
	const EntryT&	GetAt(UINT i) const { return m_row[i]; }
};

template<class _TKey, class _TValue>
class MassSet
{
public:
	typedef _TKey								KeyT;
	typedef _TValue								ValueT;
	typedef MassSetEntry<_TKey, _TValue>		EntryT;
	typedef MassSetRow<_TKey, _TValue>			EntryRowT;
	typedef MassSet<_TKey, _TValue>				SetT;

	static const UINT BucketNo = 1 << 20;

	struct Iterator
	{
		UINT		m_bucket, m_inner;
		const SetT*			m_set;
		const EntryRowT*	m_row;
		const EntryT*		m_entry;

		Iterator(SetT* pset): m_set(pset), m_bucket(0), m_inner(0), m_row(NULL), m_entry(NULL) {}
		bool Next();
		const EntryT* GetEntry() const { return m_entry; }
	};
	friend struct Iterator;

private:
	std::vector<EntryRowT*>	m_rows;
	int		m_size;

private:
	EntryRowT*		GetRowByHash(UINT hash, bool fCreate);

public:
	MassSet();
	~MassSet();

	int				Size() const { return m_size; }

	bool			AddEntry(_TKey* key, _TValue* value);
	bool			RemoveEntry(const _TKey* key);
	void			Clear();

	const Entry<_TKey, _TValue>*	GetEntry(const _TKey* key);
	const _TValue*	GetValue(const _TKey* key) { const Entry<_TKey, _TValue>* entry = GetEntry(key); return entry != NULL ? entry->Value : NULL; }

	Iterator*		BeginEnum() { return new Iterator(this); }	// Delete the iterator after enumeration done
};

typedef ValueBag<char>	StringBagA;
typedef ValueBag<WCHAR>	StringBagW;
typedef ValueBag<BYTE>	BinaryBag;

typedef Entry<StringBagA, BinaryBag>	EntryA;
typedef Entry<StringBagW, BinaryBag>	EntryW;

typedef MassSet<StringBagA, BinaryBag>	StringBinaryMapA;
typedef MassSet<StringBagW, BinaryBag>	StringBinaryMapW;

typedef MassSet<StringBagA, StringBagA> StringPairMapA;

// -------------------------------------------------------------------------- //

#include "masshashset-impl.h"

// -------------------------------------------------------------------------- //
