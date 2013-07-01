#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //
#ifdef X_OS_WINDOWS

class CriticalSection: public Object
{
	DECLARE_REF_CLASS(CriticalSection)

private:
	CRITICAL_SECTION m_CriticalSection; 

public:
	CriticalSection() { ::InitializeCriticalSection(&m_CriticalSection); }
	~CriticalSection() { ::DeleteCriticalSection(&m_CriticalSection); }
	void Enter() { ::EnterCriticalSection(&m_CriticalSection); }
	void Exit()	{ ::LeaveCriticalSection(&m_CriticalSection); }
	bool TryEnter()	{ return ::TryEnterCriticalSection(&m_CriticalSection);	}	
};

#else

#include <sched.h>

#define cpu_pause()         __asm__ ("pause")

inline UINT64 cmp_and_set(volatile UINT64 *lock, UINT64 old, UINT64 set)
{
    unsigned char   res;

    __asm__ volatile (

    "lock;            "
    "cmpxchgq  %3, %1;"
    "sete      %0;    "
    : "=a" (res) : "m" (*lock), "a" (old), "r" (set) : "cc", "memory");

    return res;
}

inline void spinlock(volatile UINT64 *lock, long value, UINT64 spin)
{
    UINT64 i, n;
    
    for ( ;; ) {
        if (*lock == 0 && cmp_and_set(lock, 0, value)) {
            return;
        }
    
        for (n = 1; n < spin; n <<= 1) {

            for (i = 0; i < n; i++) {
                cpu_pause();
            }


            if (*lock == 0 && cmp_and_set(lock, 0, value)) {
                return;
            }
        }

        sched_yield();
    }
}

class CriticalSection: public Object
{
    DECLARE_REF_CLASS(CriticalSection)

private:
    volatile UINT64   m_lock;

public:
    CriticalSection() : m_lock(0) {  }
    ~CriticalSection() { }
    void Enter() { spinlock(&m_lock, 1, 16384); } // spin count 需要调整一个合适的值。
    void Exit() { m_lock = 0; }
    bool TryEnter() { return (m_lock == 0 && cmp_and_set(&m_lock, 0, 1)); }
};

#endif
// -------------------------------------------------------------------------- //
CDEC_NS_END
