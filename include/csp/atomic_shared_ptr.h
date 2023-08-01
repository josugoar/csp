#ifndef CSP_ATOMIC_SHARED_PTR_H
#define CSP_ATOMIC_SHARED_PTR_H

#include <stdatomic.h>

#include "csp/shared_ptr.h"

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_atomic_shared_ptr_cleanup __attribute__((cleanup(csp_atomic_shared_ptr_destroy))) csp_atomic_shared_ptr
#endif
#endif

typedef void csp_atomic_shared_ptr_T;

typedef struct csp_atomic_shared_ptr csp_atomic_shared_ptr;

typedef csp_shared_ptr value_type;

constexpr auto csp_atomic_shared_ptr_is_always_lock_free = false;

void csp_atomic_shared_ptr_init(csp_atomic_shared_ptr *_this);

void csp_atomic_shared_ptr_init_s_copy(csp_atomic_shared_ptr *_this, csp_shared_ptr _desired);

void csp_atomic_shared_ptr_destroy(csp_atomic_shared_ptr *_this);

bool csp_atomic_shared_ptr_is_lock_free(const csp_atomic_shared_ptr *_this);

csp_shared_ptr csp_atomic_shared_ptr_load(const csp_atomic_shared_ptr *_this);

csp_shared_ptr csp_atomic_shared_ptr_load_explicit(const csp_atomic_shared_ptr *_this, memory_order _order);

void csp_atomic_shared_ptr_store(csp_atomic_shared_ptr *_this, csp_shared_ptr _desired);

void csp_atomic_shared_ptr_store_explicit(csp_atomic_shared_ptr *_this, csp_shared_ptr _desired, memory_order _order);

csp_shared_ptr csp_atomic_shared_ptr_exchange(csp_atomic_shared_ptr *_this, csp_shared_ptr _desired);

csp_shared_ptr csp_atomic_shared_ptr_exchange_explicit(csp_atomic_shared_ptr *_this, csp_shared_ptr _desired, memory_order _order);

bool csp_atomic_shared_ptr_compare_exchange_weak(csp_atomic_shared_ptr *_this, csp_shared_ptr *_expected, csp_shared_ptr _desired);

bool csp_atomic_shared_ptr_compare_exchange_weak_explicit(csp_atomic_shared_ptr *_this, csp_shared_ptr *_expected, csp_shared_ptr _desired, memory_order _success, memory_order _failure);

bool csp_atomic_shared_ptr_compare_exchange_strong(csp_atomic_shared_ptr *_this, csp_shared_ptr *_expected, csp_shared_ptr _desired);

bool csp_atomic_shared_ptr_compare_exchange_strong_explicit(csp_atomic_shared_ptr *_this, csp_shared_ptr *_expected, csp_shared_ptr _desired, memory_order _success, memory_order _failure);

void csp_atomic_shared_ptr_wait(const csp_atomic_shared_ptr *_this, csp_shared_ptr _old);

void csp_atomic_shared_ptr_wait_explicit(const csp_atomic_shared_ptr *_this, csp_shared_ptr _old, memory_order _order);

void csp_atomic_shared_ptr_notify_one(csp_atomic_shared_ptr *_this);

void csp_atomic_shared_ptr_notify_all(csp_atomic_shared_ptr *_this);

struct csp_atomic_shared_ptr
{
    csp_shared_ptr _p;
};

#endif
