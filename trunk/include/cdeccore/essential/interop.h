// This header contains the following:
//   - pin_ptr

#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //

template<class _Ty>
class pin_ptr
{
public:
	typedef _Ty				value_type;
	typedef _Ty*			pointer;
	typedef const _Ty*		const_pointer;
	typedef _Ty&			reference;
	typedef const _Ty*		const_reference;

	typedef pin_ptr<_Ty>	this_type;

private:
	_Ty*		_ptr;

	explicit pin_ptr(pointer ptr): _ptr(ptr) {}

public:
	pin_ptr(): _ptr(NULL) {}
	pin_ptr(const this_type& r): _ptr(r._ptr) {}
	pin_ptr(weak_ref<_Ty> r): _ptr(r.__GetPointer()) {}

	// internal usage only
	static this_type CreatePinptr(pointer ptr) { return this_type(ptr); }

	this_type& operator=(const this_type& r)
	{
		_ptr = r._ptr;
		return *this;
	}
	this_type& operator=(weak_ref<_Ty> r)
	{
		_ptr = r.__GetPointer();
		return *this;
	}
	this_type& operator=(int nil)	// NULL expected
	{
		ASSERT(nil == 0);
		_ptr = NULL;
		return *this;
	}

	bool operator==(const_pointer pRhs) const { return _ptr == pRhs; }
	bool operator!=(const_pointer pRhs) const { return _ptr != pRhs; }

	bool operator==(const this_type& rhs) const { return _ptr == rhs._ptr; }
	bool operator!=(const this_type& rhs) const { return _ptr != rhs._ptr; }

	// For C++ language doesn't provide "explicit" type conversion overloading,
	// and an implicit conversion to a pointer tends to cause latent errors, we
	// made a decision that use normal functions to meet this feature.
	//	operator pointer() const { return _ptr; }
	pointer ptr() const { return _ptr; }

	reference operator*() const { return *_ptr; }
	pointer operator->() const { return _ptr; }
};

// -------------------------------------------------------------------------- //
CDEC_NS_END
