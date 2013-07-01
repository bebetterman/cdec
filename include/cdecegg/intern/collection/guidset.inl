#pragma once

#include "guidset.h"

CDEC_EGG_NS_BEGIN

// -------------------------------------------------------------------------- //
// Notice of code migration
//
// MT support is DISABLED
// Original source from wss/base/guidset
// -------------------------------------------------------------------------- //

GuidData::GuidData(): m_cbItem(0), m_cbUid(0), m_cbData(0), m_count(0), m_capacity(0)
{
}

void GuidData::Init(UINT cbItem, UINT cbUid)
{
	m_cbItem = cbItem;
	m_cbUid = cbUid;
	m_cbData = cbItem - cbUid;

	ASSERT(cbItem >= cbUid);
	m_capPage = DATA_UNIT_SIZE / cbItem;
}

GuidData::~GuidData()
{
	for (size_t i = 0; i < m_pages.size(); ++i)
		free(m_pages[i]);
	m_pages.clear();
	m_count = m_capacity = 0;
}

BYTE* GuidData::AddUnit()
{
	BYTE* unit = (BYTE*)malloc(DATA_UNIT_SIZE);
	m_pages.push_back(unit);
	m_capacity += m_capPage;
	return unit;
}

UINT GuidData::Add(const BYTE* uid, BYTE** ppv)
{
	if (m_count >= m_capacity)
		AddUnit();

	ASSERT(m_count + m_freeIndices.size() <= m_capacity);
	UINT index = m_count++;
	if (!m_freeIndices.empty())
	{
		index = m_freeIndices.back();
		m_freeIndices.pop_back();
	}

	BYTE* p = GetAt(index);
	memcpy(p, uid, m_cbUid);
	memset(p + m_cbUid, 0, m_cbData);

	if (ppv != NULL)
		*ppv = p;
	return index;
}

void GuidData::Remove(UINT index)
{
	--m_count;
	m_freeIndices.push_back(index);
}

// -------------------------------------------------------------------------- //

// Return:
//		Index of the element in m_items
//		If found, a pair of <index of element, true> is returned
//		If not found, a pair of <first element greater, false> is returned
std::pair<UINT, bool> GuidSlide::Find(const BYTE* uid, GuidData& data, GuidTrait& trait)
{
	UINT cbUid = data.getUidLen();
	UINT l = 0, h = (UINT)m_items.size();
	while (l < h)	// [l, h)
	{
		int m = (l + h) >> 1;
		const BYTE* uid_t = data.GetAt(m_items[m]);
		switch (trait.Compare(uid_t, uid))
		{
		case 0:			// Exactly this one
			return std::pair<UINT, bool>(m, true);
		case 1:			// itm[m] > x
			h = m;		// [l, m)
			break;
		default:		// itm[m] < x
			l = m + 1;	// [m + 1, h)
			break;
		}
	}
	ASSERT(l == h);
	return std::pair<UINT, bool>(l, false);
}

void GuidSlide::__SelectRange(const BYTE* lb, const BYTE* rb, std::vector<BYTE*>& result, GuidData& data, GuidTrait& trait)
{
	std::pair<UINT, bool> rf = Find(lb, data, trait);
	UINT i = rf.first;
	UINT n = (UINT)m_items.size();
	while (i < n)
	{
		BYTE* uid_t = data.GetAt(m_items[i]);
		if (trait.Compare(uid_t, rb) > 0)
			break;
		result.push_back(uid_t);
		++i;
	}
}

bool GuidSlide::TryAdd(const BYTE* uid, BYTE** ppv, GuidData& data, GuidTrait& trait)
{
	std::pair<UINT, bool> r = Find(uid, data, trait);
	if (r.second)
	{
		if (ppv != NULL)
			*ppv = data.GetAt(m_items[r.first]);
		return false;
	}

	UINT tag = data.Add(uid, ppv);
	m_items.insert(m_items.begin() + r.first, tag);
	return true;
}

bool GuidSlide::TryRemove(const BYTE* uid, GuidData& data, GuidTrait& trait)
{
	std::pair<UINT, bool> r = Find(uid, data, trait);
	if (r.second)
	{
		UINT item = m_items[r.first];
		m_items.erase(m_items.begin() + r.first);
		data.Remove(item);
		return true;
	}
	return false;
}

// -------------------------------------------------------------------------- //

GuidSet::GuidSet(UINT cbItem, UINT cbUid, UINT prefBits, UINT offSt): m_prefBits(prefBits)
{
	m_trait = new GuidNormalTrait(cbItem, cbUid, offSt);
	_Init();
}

GuidSet::GuidSet(GuidTrait* trait, UINT prefBits): m_trait(trait), m_prefBits(prefBits)
{
	_Init();
}

void GuidSet::_Init()
{
	if (m_prefBits < 8 || m_prefBits >= 32)		// 8 - 31
		throw std::invalid_argument("Range 8-31 expected");

	m_data.Init(m_trait->m_cbItem, m_trait->m_cbUid);

	m_nBucket = 1U << m_prefBits;
	m_pBuckets = new GuidSlide*[m_nBucket];
	memset(m_pBuckets, 0, m_nBucket * sizeof(GuidSlide*));
}

GuidSet::~GuidSet()
{
	for (UINT i = 0; i < m_nBucket; ++i)
	{
		GuidSlide* p = m_pBuckets[i];
		if (p != NULL)
			delete p;
	}
	delete[] m_pBuckets;
	delete m_trait;
}

// Returns the head of the record if found, otherwise NULL
BYTE* GuidSet::Find(const BYTE* uid)
{
	UINT idx = _Find(uid);
	return idx != GUIDS_INVALID_INDEX ? m_data.GetAt(idx) : NULL;
}

UINT GuidSet::Read(BYTE* row)
{
	RWAutoLock(m_lock, false);
	UINT idx = _Find(row);
	if (idx != GUIDS_INVALID_INDEX)
	{
		BYTE* p = m_data.GetAt(idx);
		memcpy(row, p, m_data.getItemLen());
	}
	return idx;
}

bool GuidSet::Update(UINT index, const BYTE* row)
{
	RWAutoLock(m_lock, true);
	UINT cbUid = m_data.getUidLen();
	BYTE* p = m_data.GetAt(index);
	if (memcmp(p, row, cbUid) != 0)
		return false;
	memcpy(p + cbUid, row + cbUid, m_data.getItemLen() - cbUid);
	return true;
}

// ppv points to the head of the record, ie, the UID section
bool GuidSet::TryAdd(const BYTE* uid, BYTE** ppv)
{
	RWAutoLock(m_lock, true);
	UINT iBucket = _GetBucket(uid);
	GuidSlide* pSlide = m_pBuckets[iBucket];
	if (pSlide == NULL)
		pSlide = m_pBuckets[iBucket] = new GuidSlide();
	return pSlide->TryAdd(uid, ppv, m_data, *m_trait);
}

bool GuidSet::Remove(const BYTE* uid)
{
	RWAutoLock(m_lock, true);
	UINT iBucket = _GetBucket(uid);
	GuidSlide* pSlide = m_pBuckets[iBucket];
	if (pSlide == NULL)
		return false;
	return pSlide->TryRemove(uid, m_data, *m_trait);
}

void GuidSet::ReportDebugInfo()
{
	for (UINT i = 0; i < m_nBucket; ++i)
	{
		GuidSlide* p = m_pBuckets[i];
		if (p == NULL)
			continue;

		printf("%X,%d\n", i, p->getCount());
	}
}

// -------------------------------------------------------------------------- //

inline void GuidSetIter::_SeekNextSlides()
{
	m_iItem = 0;
	for (; m_iSlide < (int)m_set.m_nBucket; ++m_iSlide)
	{
		if ((m_pSlide = m_set.m_pBuckets[m_iSlide]) == NULL)
			continue;

		if (m_pSlide->m_items.size() != 0)
		{
			m_ptr = m_set.m_data.GetAt(m_pSlide->m_items.front());
			return;
		}
	}
	m_pSlide = NULL;
	m_ptr = NULL;
}

bool GuidSetIter::Reset()
{
	m_iSlide = 0;
	_SeekNextSlides();
	return Valid();
}

bool GuidSetIter::Next()
{
	if (++m_iItem < m_pSlide->m_items.size())
	{
		m_ptr = m_set.m_data.GetAt(m_pSlide->m_items[m_iItem]);
		return true;
	}

	++m_iSlide;
	_SeekNextSlides();
	return Valid();
}

// -------------------------------------------------------------------------- //

// MMAP 数据结构
// 索引部分
// OFFSET	LENGTH	COMMENT
// 0000		4		Size of index
// 0004		4		Size of data unit
// 0008		4		Count of data unit
// 000C		4		Length of row
// 0010		4		Length of uid
// 0014		4		OffSt (bit 0 - 23), PrefBit (bit 24 - 31)
// 0018		n*4		Indices of buckets
// 一般而言，索引部分按 1MB 对齐
// 从 0x18 开始，依次存放每个桶的元素个数（MMAP 中的数据已经重新排序整理，使序号连续）
//
// 数据部分
// 数据部分按照 size of data unit 对齐，顺序排列所有的行数据

struct MMapHeader
{
	DWORD	IndexSize, DUnitSize, DUnitCount, RowLength, KeyLength, OffSt;
};

void GuidSetPersist::Save(const char* path, GuidSet* pset)
{
	GuidTrait& trait = *pset->m_trait;

	// Allocating memory of index part
	int sizeIndexPart = sizeof(MMapHeader) + pset->m_nBucket * sizeof(DWORD);
	sizeIndexPart = (sizeIndexPart + DATA_UNIT_SIZE - 1) & ~(DATA_UNIT_SIZE - 1);
	BYTE*	index_part = (BYTE*)malloc(sizeIndexPart);

	// Filling header
	MMapHeader&		head = *(MMapHeader*)index_part;
	head.IndexSize = sizeIndexPart;
	head.DUnitSize = DATA_UNIT_SIZE;
	head.DUnitCount = (UINT)pset->m_data.m_pages.size();
	head.RowLength = pset->m_data.m_cbItem;
	head.KeyLength = pset->m_data.m_cbUid;
	head.OffSt = trait.m_offSt | (pset->m_prefBits << 24);
	DWORD*	pIndices = (DWORD*)(index_part + sizeof(MMapHeader));

	// Open file for write
	BYTE*	buffer = (BYTE*)malloc(DATA_UNIT_SIZE);
	FILE* fp = fopen(path, "wb");
	if (fseek(fp, sizeIndexPart, SEEK_SET) != 0)
		puts("ERR: fseek might fail");

	// Sort and saving rows
	BYTE*	pwr = buffer;
	for (UINT iSlide = 0; iSlide < pset->m_nBucket; ++iSlide)
	{
		GuidSlide* pSlide = pset->m_pBuckets[iSlide];
		if (pSlide == NULL)
		{
			*pIndices++ = 0;
			continue;
		}

		const GuidSlide::item_vector& items = pSlide->m_items;
		*pIndices++ = (DWORD)items.size();
		for (int i = 0; i < items.size(); ++i)
		{
			BYTE* pItem = pset->m_data.GetAt(items[i]);
			memcpy(pwr, pItem, head.RowLength);
			pwr += head.RowLength;
			if (pwr - buffer + head.RowLength > DATA_UNIT_SIZE)
			{
				if (fwrite(buffer, 1, DATA_UNIT_SIZE, fp) != DATA_UNIT_SIZE)
					puts("ERR: fwrite might fail");
				pwr = buffer;
			}
		}
	}
	if (pwr > buffer)
		if (fwrite(buffer, 1, DATA_UNIT_SIZE, fp) != DATA_UNIT_SIZE)
			puts("ERR: fwrite might fail");

	// Completing
	fseek(fp, 0, SEEK_SET);
	if (fwrite(index_part, 1, sizeIndexPart, fp) != sizeIndexPart)
		puts("ERR: fwrite might fail");
	fclose(fp);
	free(buffer);
	free(index_part);
}

void GuidSetPersist::Load(const char* path, GuidSet* pset)
{
	// Reading header
	MMapHeader head;
	FILE* fp = fopen(path, "rb");
	if (fread(&head, sizeof(head), 1, fp) != 1)
		puts("ERR: fread might fail");

	// Reading index part
	BYTE* buffer = (BYTE*)malloc(head.IndexSize);
	DWORD* pr = (DWORD*)(buffer + sizeof(MMapHeader));
	UINT ntotal = 0;
	if (fseek(fp, 0, SEEK_SET) != 0)
		puts("ERR: seek might fail");
	if (fread(buffer, 1, head.IndexSize, fp) != head.IndexSize)
		puts("ERR: fread might fail");
	for (UINT iSlide = 0; iSlide < pset->m_nBucket; ++iSlide)
	{
		UINT nitem = *pr++;
		if (nitem == 0)
			continue;

		GuidSlide* pSlide = pset->m_pBuckets[iSlide];
		if (pSlide == NULL)
			pset->m_pBuckets[iSlide] = pSlide = new GuidSlide();

		pSlide->m_items.resize(nitem);
		for (UINT i = 0; i < nitem; ++i)
			pSlide->m_items[i] = ntotal++;
	}
	free(buffer);

	// Reading data
	for (UINT i = 0; i < head.DUnitCount; ++i)
	{
		BYTE* pUnit = pset->m_data.AddUnit();
		if (fread(pUnit, 1, DATA_UNIT_SIZE, fp) != DATA_UNIT_SIZE)
			puts("ERR: fread might fail");
	}

	// Completing
	pset->m_data.m_count = ntotal;
	fclose(fp);
}

void GuidSetPersist::Verify(GuidSet* pset)
{
	puts("Begin checking");
	GuidData& data = pset->m_data;
	GuidTrait& trait = *pset->m_trait;
	UINT	nTotal = 0, nCount = data.m_count;
	UINT	cbUid = data.m_cbUid, prefbit = pset->m_prefBits, offSt = trait.m_offSt;
	for (UINT iSlide = 0; iSlide < pset->m_nBucket; ++iSlide)
	{
		GuidSlide* p = pset->m_pBuckets[iSlide];
		if (p == NULL)
			continue;

		GuidSlide::item_vector& items = p->m_items;
		nTotal += (UINT)items.size();
		BYTE*	pLast = NULL;
		for (UINT i = 0; i < items.size(); ++i)
		{
			UINT ic = items[i];
			if (ic >= nCount)
				puts("ERR: Index >= Count");

			BYTE* pCur = data.GetAt(ic);
			if (pset->_GetBucket(pCur) != iSlide)
				puts("ERR: Incorrect slide");
			if (i > 0 && trait.Compare(pLast, pCur) >= 0)
				puts("ERR: Invalid order");
			pLast = pCur;
		}
	}
	if (nTotal != nCount)
		puts("ERR: Count not equal");
	puts("Complete checking");
}

// -------------------------------------------------------------------------- //

CDEC_EGG_NS_END
