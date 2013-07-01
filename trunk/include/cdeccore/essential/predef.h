// -------------------------------------------------------------------------- //
//	文件名		：	kfc/cdec/cdec_base.h
//	创建者		：	杨钢
//	创建时间	：	5/9/2008 10:49:36 AM
//	功能描述	：	
//
// -------------------------------------------------------------------------- //

#pragma once

// -------------------------------------------------------------------------- //
// Memory management
// -------------------------------------------------------------------------- //

// Normal allocation behavior, used for large quantity of small objects.
//
// This is an alternated behavior than a normal one, that might lower 
// overhead cost while allocating objects, by specifying the length of a 
// block when it's being freed. Because a memory pool such as KFC Memfix 
// Pool Service, would no longer need to keep a header beside the block
// allocated.
// While if the length specified meets the criteria of a large memory 
// request, this function might (also might not) invoke the LC allocation
// versions.
CDECCOREAPI(void*)		CoreAllocT(size_t cbSize);
CDECCOREAPI(void)		CoreFreeT(void* pv, size_t cbSize);

// Allocation for a large-scale contiguous memory block.
// 
// The length of a request is not limited on semantic level. But on a 
// spcified implementation, it might mean a low performance or waste of 
// space while an unsuitable size is requested. As a general recommendation,
// you're encouraged to use a value divisible by the platform's page size 
// (4KB on X86 archetecture), or use an integer power of 2 that over 64 
// bytes (e.g. 128, 256, etc).
CDECCOREAPI(void*)		CoreAllocLC(size_t cbSize);
CDECCOREAPI(void)		CoreFreeLC(void* pv, size_t cbSize);

// -------------------------------------------------------------------------- //
