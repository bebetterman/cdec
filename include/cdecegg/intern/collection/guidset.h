#pragma once

CDEC_EGG_NS_BEGIN

// -------------------------------------------------------------------------- //
// GuidSet
//
// GuidSet is not a C-- reference class for performance issue
// -------------------------------------------------------------------------- //
// Notice of code migration
//
// MT support is DISABLED
// Original source from wss/base/guidset
//
// Iterator has been modified to compliant with C--
// It should be adapted used on the former JNI implements
//
// Updated 2012/4/11
// Supports user-defined comparision
//
// Updated 2012/8/7
// Moved into CdecCore project
// -------------------------------------------------------------------------- //

class RWLock
{
public:
	RWLock() { }
	void	LockRead() { }
	void	LockWrite() { }
	void	Unlock() { }
};

struct RWAutoLock
{
	RWLock& m_lock;

	RWAutoLock(RWLock& lock, bool fWrite): m_lock(lock)
	{
		if (fWrite) m_lock.LockWrite(); else m_lock.LockRead();
	}
	~RWAutoLock() { m_lock.Unlock(); }
};

// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// Comments for multi-threading (MT) supporting
//
// Method comment with "ST" means that method does not supports multi-threading,
// and you must not call it in a parallel scene.
// Neither Constructor or destructor supports multi-threading
//
// Class GuidSetIter doesn't support multi-threading.
// Class GuidSetPersist mostly doesn't support MT except SaveMT method.
//
// -------------------------------------------------------------------------- //

const UINT GUIDS_INVALID_INDEX = (UINT)-1;
const UINT DATA_UNIT_SIZE = 0x100000 * 4;

struct GuidTrait
{
	UINT m_cbItem, m_cbUid, m_offSt;
	GuidTrait(UINT cbItem, UINT cbUid, UINT offSt): m_cbItem(cbItem), m_cbUid(cbUid), m_offSt(offSt) {}

	virtual int Compare(const BYTE* lhs, const BYTE* rhs) = 0;
	virtual UINT Hash(const BYTE* uid) = 0;

	virtual ~GuidTrait() {}
};

struct GuidNormalTrait: GuidTrait
{
	GuidNormalTrait(UINT cbItem, UINT cbUid, UINT offSt): GuidTrait(cbItem, cbUid, offSt) {}

	int Compare(const BYTE* lhs, const BYTE* rhs);
	UINT Hash(const BYTE* uid) { return *(const UINT*)(uid + m_offSt); }
};

// -------------------------------------------------------------------------- //

class GuidData
{
	UINT		m_cbItem, m_cbUid, m_cbData, m_capPage;	
	UINT		m_count, m_capacity;
	std::vector<BYTE*>	m_pages;
	std::deque<UINT>	m_freeIndices;

public:
	GuidData();
	void	Init(UINT cbItem, UINT cbUid);
	~GuidData();

	BYTE*	GetAt(UINT index)
	{
		ASSERT(index < m_count + m_freeIndices.size());
		return m_pages[index / m_capPage] + index % m_capPage * m_cbItem;
	}

	UINT	Add(const BYTE* uid, BYTE** ppv);
	void	Remove(UINT index);

	UINT	getUidLen() { return m_cbUid; }
	UINT	getItemLen() { return m_cbItem; }
	UINT	getCount() { return m_count; }

private:
	BYTE*	AddUnit();
	friend class GuidSetPersist;
};

// -------------------------------------------------------------------------- //

class GuidSlide
{
	typedef std::vector<UINT> item_vector;
	item_vector		m_items;

	std::pair<UINT, bool>	Find(const BYTE* uid, GuidData& data, GuidTrait& trait);

public:
	UINT	FindItem(const BYTE* uid, GuidData& data, GuidTrait& trait);
	void	__SelectRange(const BYTE* lb, const BYTE* rb, std::vector<BYTE*>& result, GuidData& data, GuidTrait& trait);

	bool	TryAdd(const BYTE* uid, BYTE** ppv, GuidData& data, GuidTrait& trait);
	bool	TryRemove(const BYTE* uid, GuidData& data, GuidTrait& trait);

	UINT	getCount() { return (UINT)m_items.size(); }

	friend class GuidSetIter;
	friend class GuidSetPersist;
};

// -------------------------------------------------------------------------- //

class GuidSet
{
	GuidData		m_data;
	GuidTrait*		m_trait;

	UINT			m_prefBits;
	UINT			m_nBucket;
	GuidSlide**		m_pBuckets;

	RWLock			m_lock;

private:
	UINT	_GetBucket(const BYTE* uid) { return m_trait->Hash(uid) & (m_nBucket - 1); }
	UINT	_Find(const BYTE* uid);

	void	_Init();

public:
	GuidSet(UINT cbItem, UINT cbUid, UINT prefBits, UINT offSt);
	GuidSet(GuidTrait* trait, UINT prefBits);
	~GuidSet();

	BYTE*	Find(const BYTE* uid);	// ST

	void	__SelectRange(const BYTE* lb, const BYTE* rb, std::vector<BYTE*>& result);

	UINT	Read(BYTE* row);
	bool	Update(UINT index, const BYTE* row);

	bool	TryAdd(const BYTE* uid, BYTE** ppv);
	bool	Remove(const BYTE* uid);

	UINT	getUidLen() { return m_data.getUidLen(); }
	UINT	getItemLen() { return m_data.getItemLen(); }
	UINT	getCount() { return m_data.getCount(); }

	// Only used in single-threading mode
	void	VerifyThis();		// ST
	void	ReportDebugInfo();	// ST

	friend class GuidSetIter;
	friend class GuidSetPersist;
};

// -------------------------------------------------------------------------- //

class GuidSetIter	// ST
{
	GuidSet&		m_set;
	GuidSlide*		m_pSlide;
	UINT	m_iSlide, m_iItem;
	BYTE*	m_ptr;

public:
	GuidSetIter(GuidSet& set): m_set(set) { Reset(); }

	BYTE*	Current() { return m_ptr; }
	bool	Valid() { return m_ptr != NULL; }

	bool	Reset();
	bool	Next();

private:
	inline void _SeekNextSlides();
};

// -------------------------------------------------------------------------- //

class GuidSetPersist	// ST
{
public:
	static void Save(const char* path, GuidSet* pset);
	static void Load(const char* path, GuidSet* pset);
	static void Verify(GuidSet* pset);
};

// -------------------------------------------------------------------------- //

inline int GuidNormalTrait::Compare(const BYTE* lhs, const BYTE* rhs)
{
	for (UINT i = m_offSt; i < m_cbUid; ++i)
	{
		BYTE	a = lhs[i], b = rhs[i];
		if (a > b)
			return 1;
		else if (a < b)
			return -1;
	}
	for (UINT i = 0; i < m_offSt; ++i)
	{
		BYTE	a = lhs[i], b = rhs[i];
		if (a > b)
			return 1;
		else if (a < b)
			return -1;
	}
	return 0;
}

// -------------------------------------------------------------------------- //

inline UINT GuidSlide::FindItem(const BYTE* uid, GuidData& data, GuidTrait& trait)
{
	std::pair<UINT, bool> r = Find(uid, data, trait);
	return r.second ? m_items[r.first] : GUIDS_INVALID_INDEX;
}

inline UINT GuidSet::_Find(const BYTE* uid)
{
	GuidSlide* pSlide = m_pBuckets[_GetBucket(uid)];
	return pSlide != NULL ? pSlide->FindItem(uid, m_data, *m_trait) : GUIDS_INVALID_INDEX;
}

inline void GuidSet::__SelectRange(const BYTE* lb, const BYTE* rb, std::vector<BYTE*>& result)
{
	GuidSlide* pSlide = m_pBuckets[_GetBucket(lb)];
	if (pSlide != NULL)
		pSlide->__SelectRange(lb, rb, result, m_data, *m_trait);
}

// -------------------------------------------------------------------------- //

CDEC_EGG_NS_END
