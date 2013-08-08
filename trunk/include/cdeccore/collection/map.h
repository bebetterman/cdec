#pragma once

CDEC_NS_BEGIN
//---------------------------------------------------------------------------//

template<class _K, class _V>
class SortedMapVV: public Object
{
	DECLARE_REF_CLASS(TreeMapVV(_K, _V))

public:
	typedef _K								key_type;
	typedef _V								value_type;
	typedef std::pair<_K, _V>				KeyValuePair;

	typedef std::map<_K, _V>				impl_type;
	typedef typename impl_type::iterator	iterator_type;

	typedef TraitIteratorValue<iterator_type>	enum_trait_type;
	
	struct enum_trait_key_type
	{
		typedef typename SortedMapVV<_K, _V>::iterator_type iterator_type;
		typedef _K value_type;
		static value_type Get(iterator_type it) { return it->first; }
	};

	struct enum_trait_value_type
	{
		typedef typename SortedMapVV<_K, _V>::iterator_type iterator_type;
		typedef _V value_type;
		static value_type Get(iterator_type it) { return it->second; }
	};

	typedef _IteratorTraitEnum<KeyValuePair, enum_trait_type>		key_value_enum;
	typedef _IteratorTraitEnum<_K, enum_trait_key_type>				key_enum;
	typedef _IteratorTraitEnum<_V, enum_trait_value_type>			value_enum;

protected:
	impl_type	m_impl;
	_V			m_nil;

public:
	SortedMapVV(): m_nil(_V()) {}

	int		Count() { return m_impl.size(); }

	bool	HasKey(const _K& key)
	{
		iterator_type it = m_impl.find(key);
		return it != m_impl.end();
	}

	// If specified key exists, returns the value. Otherwise, returns a default Value
	// This method does not modify the collection in any case
	const _V&	Get(const _K& key)
	{
		iterator_type it = m_impl.find(key);
		return it != m_impl.end() ? it->second : m_nil;
	}
	
	// Similar to Get, but exception raised when specified key not found
	_V&		at(const _K& key)
	{
		iterator_type it = m_impl.find(key);
		if (it == m_impl.end())
			cdec_throw(Exception(EC_KeyNotFound));
		return it->second;
	}

	// If specified key exists, parameter Val is set the value, and returns true
	// Otherwise, returns false, and the value of parameter Val is a default Value
	// This method does not modify the collection in any case
	bool	TryGet(const _K& key, _V& val)
	{
		iterator_type it = m_impl.find(key);
		if (it != m_impl.end())
		{
			val = it->second;
			return true;
		}
		else
		{
			val = _V();
			return false;
		}
	}

	// If specified key exists, overwrites the value and returns false. 
	// Otherwise, inserts a new element and returns true.
	bool	Set(const _K& key, const _V& val)
	{
		std::pair<iterator_type, bool> r = m_impl.insert(typename impl_type::value_type(key, val));
		if (!r.second)
			r.first->second = val;
		return r.second;
	}

	// If specified key exists, the function returns false and does nothing.
	// Otherwise, inserts a new element and returns true
	bool	Insert(const _K& key, const _V& val)
	{
		std::pair<iterator_type, bool> r = m_impl.insert(typename impl_type::value_type(key, val));
		return r.second;
	}

	// If specified key exists, deletes the element and returns true. Otherwise, returns false.
	bool	Remove(const _K& key) { return m_impl.erase(key) != 0; }

	void	Clear() { m_impl.clear(); }

	ref<IEnumerator<KeyValuePair> > GetEnumerator()
	{
		return gc_new<key_value_enum>(m_impl.begin(), m_impl.end());
	}

	ref<IEnumerable<KeyValuePair> > Enum()
	{
		ref<IEnumerator<KeyValuePair> > e = GetEnumerator();
		return gc_new<Enumerable<KeyValuePair> >(e);
	}

	ref<IEnumerable<KeyValuePair> > EnumRange(const _K& first, const _K& last)
	{
		iterator_type itf = m_impl.lower_bound(first);
		iterator_type itl = m_impl.upper_bound(last);
		ref<IEnumerator<KeyValuePair> > e = gc_new<key_value_enum>(itf, itl);
		return gc_new<Enumerable<KeyValuePair> >(e);
	}

	ref<IEnumerable<_K> > EnumKeys()
	{
		ref<IEnumerator<_K> > e = gc_new<key_enum>(m_impl.begin(), m_impl.end());
		return gc_new<Enumerable<_K> >(e);
	}

	ref<IEnumerable<_K> > EnumKeysRange(const _K& first, const _K& last)
	{
		iterator_type itf = m_impl.lower_bound(first);
		iterator_type itl = m_impl.upper_bound(last);
		ref<IEnumerator<_K> > e = gc_new<key_enum>(itf, itl);
		return gc_new<Enumerable<_K> >(e);
	}

	ref<IEnumerable<_V> > EnumValues()
	{
		ref<IEnumerator<_V> > e = gc_new<value_enum>(m_impl.begin(), m_impl.end());
		return gc_new<Enumerable<_V> >(e);
	}

	ref<IEnumerable<_V> > EnumValuesRange(const _K& first, const _K& last)
	{
		iterator_type itf = m_impl.lower_bound(first);
		iterator_type itl = m_impl.upper_bound(last);
		ref<IEnumerator<_V> > e = gc_new<value_enum>(itf, itl);
		return gc_new<Enumerable<_V> >(e);
	}
};

//---------------------------------------------------------------------------//

template<class _K, class _V>
class SortedMapVR: public SortedMapVV<_K, ref<_V> >
{
};

//---------------------------------------------------------------------------//
CDEC_NS_END
