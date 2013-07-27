// This header contains the following:
//   - variant_core
//	 - variantx

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

struct variant_core
{
	UINT64 Value;

	variant_core(): Value(NULL)
	{
		ASSERT(sizeof(bool) == sizeof(BYTE));
		ASSERT(sizeof(double) == sizeof(Value));
		ASSERT(sizeof(void*) <= sizeof(Value));
		ASSERT(sizeof(stringx) == sizeof(void*));
		ASSERT(sizeof(ref<Object>) == sizeof(void*));
	}

	void	SetBool(bool value)	{ (bool&)Value = value; }

	void	SetInt8 (INT8 value)  { (INT8&) Value = value; }
	void	SetInt16(INT16 value) { (INT16&)Value = value; }
	void	SetInt32(INT32 value) { (INT32&)Value = value; }
	void	SetInt64(INT64 value) { (INT64&)Value = value; }

	void	SetUInt8 (UINT8 value)  { (UINT8&)Value = value; }
	void	SetUInt16(UINT16 value) { (UINT16&)Value = value; }
	void	SetUInt32(UINT32 value) { (UINT32&)Value = value; }
	void	SetUInt64(UINT64 value) { (UINT64&)Value = value; }

	void	SetDouble(double value) { (double&)Value = value; }

	void	SetPtr(void* ptr) { (void*&)Value = ptr; }

	void	SetString(stringx value) { (stringx&)Value = value; }

	void	SetObject(ref<Object> value) { (ref<Object>&)Value = value; }

	bool	GetBool() { return (bool&)Value; }

	INT8	GetInt8()  { return (INT8&) Value; }
	INT16	GetInt16() { return (INT16&)Value; }
	INT32	GetInt32() { return (INT32&)Value; }
	INT64	GetInt64() { return (INT64&)Value; }

	UINT8	GetUInt8()  { return (UINT8&) Value; }
	UINT16	GetUInt16() { return (UINT16&)Value; }
	UINT32	GetUInt32() { return (UINT32&)Value; }
	UINT64	GetUInt64() { return (UINT64&)Value; }

	double	GetDouble() { return (double&)Value; }

	void*	GetPtr() { return (void*&)Value; }

	const stringx&	GetString() { return (const stringx&)Value; }

	const ref<Object>&	GetObject() { return (const ref<Object>&)Value; }

	template<typename T>
	void GetObject(ref<T>& r) { r = (const ref<Object>&)Value; }

	// ClearLiteral works for the following types:
	// - integers (INT / UINT | 8 / 16 / 32 / 64)
	// - bool
	// - ptr (void*)
	void	ClearLiteral() { Value = 0; }

	void	ClearString() { ((stringx&)Value).Clear(); }

	void	ClearObject() { (ref<Object>&)Value = NULL; }
};

// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //

CDEC_NS_END
