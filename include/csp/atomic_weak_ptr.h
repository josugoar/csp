#ifndef CSP_ATOMIC_WEAK_PTR_H
#define CSP_ATOMIC_WEAK_PTR_H

#include <stdatomic.h>

#include "csp/exception.h"
#include "csp/weak_ptr.h"

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_atomic_weak_ptr_cleanup __attribute__((cleanup(csp_atomic_weak_ptr_destroy))) csp_atomic_weak_ptr
#endif
#endif

typedef void csp_atomic_weak_ptr_T;

/// @brief Atomic shared pointer.
typedef struct csp_atomic_weak_ptr csp_atomic_weak_ptr;

/// @brief csp_weak_ptr.
typedef csp_weak_ptr csp_atomic_weak_ptr_value_type;

constexpr auto csp_atomic_weak_ptr_is_always_lock_free = false;

/// @brief Initializes the underlying csp_weak_ptr to empty value.
csp_atomic_weak_ptr csp_atomic_weak_ptr_init(void);

/// @brief Initializes the underlying csp_weak_ptr to a copy of desired. As with any csp_atomic type, initialization is not an atomic operation.
csp_atomic_weak_ptr csp_atomic_weak_ptr_init_w(csp_weak_ptr _desired);

/// @brief Destroy the underlying csp_weak_ptr.
void csp_atomic_weak_ptr_destroy(const csp_atomic_weak_ptr* _this);

/// @brief Returns true if the atomic operations on all objects of this type are lock-free, false otherwise.
bool csp_atomic_weak_ptr_is_lock_free(const csp_atomic_weak_ptr* _this);

/// @brief Atomically returns a copy of the underlying csp_weak_ptr.
csp_weak_ptr csp_atomic_weak_ptr_load(const csp_atomic_weak_ptr* _this);

/// @brief Atomically returns a copy of the underlying csp_weak_ptr. Memory is ordered according to order. The behavior is undefined if order is memory_order_release or memory_order_acq_rel.
csp_weak_ptr csp_atomic_weak_ptr_load_explicit(const csp_atomic_weak_ptr* _this, memory_order _order);

/// @brief Atomically replaces the value of this with the value of desired as if by csp_weak_ptr_swap(_this->_r, &desired) where _r is the underlying csp_weak_ptr.
void csp_atomic_weak_ptr_store(csp_atomic_weak_ptr* _this, csp_weak_ptr _desired);

/// @brief Atomically replaces the value of this with the value of desired as if by csp_weak_ptr_swap(_this->_r, &desired) where _r is the underlying csp_weak_ptr. Memory is ordered according to order. The behavior is undefined if order is memory_order_consume, memory_order_acquire, or memory_order_acq_rel.
void csp_atomic_weak_ptr_store_explicit(csp_atomic_weak_ptr* _this, csp_weak_ptr _desired, memory_order _order);

/// @brief Atomically replaces the underlying csp_weak_ptr with desired as if by csp_weak_ptr_swap(_this->_r, &desired) where _r is the underlying csp_weak_ptr, and returns a copy of the value that p had immediately before the swap. This is an atomic read-modify-write operation.
csp_weak_ptr csp_atomic_weak_ptr_exchange(csp_atomic_weak_ptr* _this, csp_weak_ptr _desired);

/// @brief Atomically replaces the underlying csp_weak_ptr with desired as if by csp_weak_ptr_swap(_this->_r, &desired) where _r is the underlying csp_weak_ptr, and returns a copy of the value that p had immediately before the swap. Memory is ordered according to order. This is an atomic read-modify-write operation.
csp_weak_ptr csp_atomic_weak_ptr_exchange_explicit(csp_atomic_weak_ptr* _this, csp_weak_ptr _desired, memory_order _order);

/// @brief Equivalent to: return csp_atomic_weak_ptr_compare_exchange_weak(_this, _expected, _desired, memory_order_seq_cst, memory_order_seq_cst).
bool csp_atomic_weak_ptr_compare_exchange_weak(csp_atomic_weak_ptr* _this, csp_weak_ptr* _expected, csp_weak_ptr _desired);

/// @brief Same as csp_atomic_weak_ptr_compare_exchange_strong_explicit, but may also fail spuriously.
bool csp_atomic_weak_ptr_compare_exchange_weak_explicit(csp_atomic_weak_ptr* _this, csp_weak_ptr* _expected, csp_weak_ptr _desired, memory_order _success, memory_order _failure);

/// @brief Equivalent to: return csp_atomic_weak_ptr_compare_exchange_strong_explicit(_this, _expected, _desired, memory_order_seq_cst, memory_order_seq_cst).
bool csp_atomic_weak_ptr_compare_exchange_strong(csp_atomic_weak_ptr* _this, csp_weak_ptr* _expected, csp_weak_ptr _desired);

/// @brief If the underlying csp_weak_ptr stores the same pointer value as expected and shares ownership with it, or if both underlying and expected are empty, assigns from desired to the underlying csp_weak_ptr, returns true, and orders memory according to success, otherwise assigns from the underlying csp_weak_ptr to expected, returns false, and orders memory according to failure. The behavior is undefined if failure is memory_order_release or memory_order_acq_rel. On success, the operation is an atomic read-modify-write operation on this and expected is not accessed after the atomic update. On failure, the operation is an atomic load operation on this and expected is updated with the existing value read from the atomic object. This update to expected's csp_weak_ptr_use_count is part of this atomic operation, although the write itself (and any subsequent deallocation/destruction) is not required to be.
bool csp_atomic_weak_ptr_compare_exchange_strong_explicit(csp_atomic_weak_ptr* _this, csp_weak_ptr* _expected, csp_weak_ptr _desired, memory_order _success, memory_order _failure);

/// @brief Equivalent to: csp_atomic_weak_ptr_wait_explicit(_this, _old, memory_order_seq_cst).
void csp_atomic_weak_ptr_wait(const csp_atomic_weak_ptr* _this, const csp_weak_ptr* _old);

/// @brief Performs an atomic waiting operation. Compares csp_atomic_weak_ptr_load_explicit(_this, _order) with _old and if they are equivalent then blocks until _this is notified by csp_atomic_weak_ptr_notify_one() or csp_atomic_weak_ptr_notify_all(). This is repeated until csp_atomic_weak_ptr_load(_this, _order) changes. This function is guaranteed to return only if value has changed, even if underlying implementation unblocks spuriously. Memory is ordered according to _order. The behavior is undefined if order is memory_order_release or memory_order_acq_rel. Notes: two csp_weak_ptrs are equivalent if they store the same pointer and either share ownership or are both empty.
void csp_atomic_weak_ptr_wait_explicit(const csp_atomic_weak_ptr* _this, const csp_weak_ptr* _old, memory_order _order);

/// @brief Performs an atomic notifying operation. If there is a thread blocked in atomic waiting operations(i.e. csp_atomic_weak_ptr_wait()) on _this, then unblocks at least one such thread; otherwise does nothing.
void csp_atomic_weak_ptr_notify_one(const csp_atomic_weak_ptr* _this);

/// @brief Performs an atomic notifying operation. Unblocks all threads blocked in atomic waiting operations(i.e. csp_atomic_weak_ptr_wait()) on* this, if there are any; otherwise does nothing.
void csp_atomic_weak_ptr_notify_all(const csp_atomic_weak_ptr* _this);

struct csp_atomic_weak_ptr
{
    csp_weak_ptr _r;
};

#endif
