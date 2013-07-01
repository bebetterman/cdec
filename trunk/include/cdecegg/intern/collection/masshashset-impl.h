#pragma once

// -------------------------------------------------------------------------- //

template<class _TKey, class _TValue>
int compare_entry_setspc(const MassSetEntry<_TKey, _TValue>& lhs, const MassSetEntry<_TKey, _TValue>& rhs)
{
	if (lhs.Hash < rhs.Hash)
		return -1;
	if (lhs.Hash > rhs.Hash)
		return 1;

	const _TKey &lk = *lhs.Key, &rk = *rhs.Key;
	if (lk.Len() < rk.Len())
		return -1;
	if (lk.Len() > rk.Len())
		return 1;

	for (UINT i = 0; i < lk.Len(); ++i)
	{
		typename _TKey::value_type lc = lk.GetAt(i), rc = rk.GetAt(i);
		if (lc < rc)
			return -1;
		if (lc > rc)
			return 1;
	}

	return 0;	// Equal
}

template<class _TKey, class _TValue>
inline bool less_entry_setspc(const MassSetEntry<_TKey, _TValue>& lhs, const MassSetEntry<_TKey, _TValue>& rhs)
{
	return compare_entry_setspc<_TKey, _TValue>(lhs, rhs) < 0;
}

template<class _TKey, class _TValue>
bool MassSetRow<_TKey, _TValue>::AddEntry(UINT hash, _TKey* key, _TValue* value)
{
	EntryT e(hash, key, value);
	typename EntryVectorT::iterator it = std::lower_bound(m_row.begin(), m_row.end(), e, less_entry_setspc<_TKey, _TValue>);
	if (it != m_row.end() && compare_entry_setspc(e, *it) == 0)
	{
		_TKey::Delete(key);
		_TValue::Delete(value);
		return false;
	}
	m_row.insert(it, e);
	return true;
}

template<class _TKey, class _TValue>
Entry<_TKey, _TValue>* MassSetRow<_TKey, _TValue>::GetEntry(UINT hash, const _TKey* key)
{
	EntryT e(hash, const_cast<_TKey*>(key), NULL);
	typename EntryVectorT::iterator it = std::lower_bound(m_row.begin(), m_row.end(), e, less_entry_setspc<_TKey, _TValue>);
	if (it != m_row.end() && compare_entry_setspc(e, *it) == 0)
		return &*it;
	else
		return NULL;
}

template<class _TKey, class _TValue>
bool MassSetRow<_TKey, _TValue>::RemoveEntry(UINT hash, const _TKey* key)
{
	EntryT e(hash, const_cast<_TKey*>(key), NULL);
	typename EntryVectorT::iterator it = std::lower_bound(m_row.begin(), m_row.end(), e, less_entry_setspc<_TKey, _TValue>);
	if (it != m_row.end() && compare_entry_setspc(e, *it) == 0)
	{
		it->Delete();
		m_row.erase(it);
		return true;
	}
	else
		return false;
}

template<class _TKey, class _TValue>
void MassSetRow<_TKey, _TValue>::Clear()
{
	for (typename EntryVectorT::iterator it = m_row.begin(); it != m_row.end(); ++it)
		it->Delete();
	// m_row = std::vector<EntryT>();	// Release the buffer of vector
	m_row.clear();
}

// -------------------------------------------------------------------------- //

template<class _TKey, class _TValue>
MassSet<_TKey, _TValue>::MassSet(): m_size(0)
{
	m_rows.resize(BucketNo, NULL);
}

template<class _TKey, class _TValue>
MassSet<_TKey, _TValue>::~MassSet()
{
	for (typename std::vector<EntryRowT*>::iterator it = m_rows.begin(); it != m_rows.end(); ++it)
		delete *it;
	m_rows.clear();
}

template<class _TKey, class _TValue>
MassSetRow<_TKey, _TValue>* MassSet<_TKey, _TValue>::GetRowByHash(UINT hash, bool fCreate)
{
	UINT ibucket = hash & (BucketNo - 1);
	EntryRowT* row = m_rows[ibucket];
	if (fCreate && row == NULL)
		row = m_rows[ibucket] = new EntryRowT();
	return row;
}

template<class _TKey, class _TValue>
bool MassSet<_TKey, _TValue>::AddEntry(_TKey* key, _TValue* value)
{
	UINT hash = key->GetHash();
	EntryRowT* row = GetRowByHash(hash, true);

	bool f = row->AddEntry(hash, key, value);
	if (f)
		++m_size;
	return f;
}

template<class _TKey, class _TValue>
const Entry<_TKey, _TValue>* MassSet<_TKey, _TValue>::GetEntry(const _TKey* key)
{
	UINT hash = key->GetHash();
	EntryRowT* row = GetRowByHash(hash, false);
	if (row == NULL)
		return NULL;

	return row->GetEntry(hash, key);
}

template<class _TKey, class _TValue>
bool MassSet<_TKey, _TValue>::RemoveEntry(const _TKey* key)
{
	UINT hash = key->GetHash();
	EntryRowT* row = GetRowByHash(hash, false);
	if (row == NULL)
		return false;

	bool f = row->RemoveEntry(hash, key);
	if (f)
		--m_size;
	return f;
}

template<class _TKey, class _TValue>
void MassSet<_TKey, _TValue>::Clear()
{
	for (typename std::vector<EntryRowT*>::iterator it = m_rows.begin(); it != m_rows.end(); ++it)
	{
		EntryRowT* row = *it;
		if (row != NULL)
			row->Clear();
	}
	m_size = 0;
}

// -------------------------------------------------------------------------- //

template<class _TKey, class _TValue>
bool MassSet<_TKey, _TValue>::Iterator::Next()
{
	if (m_bucket >= BucketNo)
		return false;

	// Next element in this row
	if (m_row != NULL && ++m_inner < m_row->Size())
	{
		m_entry = &m_row->GetAt(m_inner);
		return true;
	}
	
	// Next row contains element
	for (m_row != NULL ? ++m_bucket : 0; m_bucket < BucketNo; ++m_bucket)
	{
		if (m_set->m_rows[m_bucket] == NULL)
			continue;

		m_row = m_set->m_rows[m_bucket];
		if (m_row->Size() != 0)
		{
			m_entry = &m_row->GetAt(m_inner = 0);
			return true;
		}
	}
	return false;
}
