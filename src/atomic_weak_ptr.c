#include "csp/atomic_weak_ptr.h"

#include <assert.h>
#include <threads.h>

void csp_atomic_weak_ptr_init(csp_atomic_weak_ptr *const _this)
{
    assert(_this);

    csp_weak_ptr_init(&_this->_p);
}

void csp_atomic_weak_ptr_init_w_copy(csp_atomic_weak_ptr *const _this, const csp_weak_ptr _desired)
{
    assert(_this);

    csp_weak_ptr_init_w_copy(&_this->_p, &_desired);
}

void csp_atomic_weak_ptr_destroy(csp_atomic_weak_ptr *const _this)
{
    assert(_this);

    csp_weak_ptr_destroy(&_this->_p);
}

bool csp_atomic_weak_ptr_is_lock_free(const csp_atomic_weak_ptr *const _this)
{
    assert(_this);

    return false;
}

csp_weak_ptr csp_atomic_weak_ptr_load(const csp_atomic_weak_ptr *const _this)
{
    assert(_this);

    csp_weak_ptr _r;

    mtx_t _mtx;

    if (mtx_init(&_mtx, mtx_plain) != thrd_success)
    {
        // TODO: error handling
    }

    if (mtx_lock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    csp_weak_ptr_init_w_copy(&_r, &_this->_p);

    if (mtx_unlock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    mtx_destroy(&_mtx);

    return _r;
}

csp_weak_ptr csp_atomic_weak_ptr_load_explicit(const csp_atomic_weak_ptr *const _this, const memory_order _order)
{
    assert(_this);
    assert(_order != memory_order_release && _order != memory_order_acq_rel);

    return csp_atomic_weak_ptr_load(_this);
}

void csp_atomic_weak_ptr_store(csp_atomic_weak_ptr *const _this, csp_weak_ptr _desired)
{
    assert(_this);

    mtx_t _mtx;

    if (mtx_init(&_mtx, mtx_plain) != thrd_success)
    {
        // TODO: error handling
    }

    if (mtx_lock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    csp_weak_ptr_swap(&_this->_p, &_desired);

    if (mtx_unlock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    mtx_destroy(&_mtx);
}

void csp_atomic_weak_ptr_store_explicit(csp_atomic_weak_ptr *const _this, const csp_weak_ptr _desired, const memory_order _order)
{
    assert(_this);
    assert(_order != memory_order_consume && _order != memory_order_acquire && _order != memory_order_acq_rel);

    csp_atomic_weak_ptr_store(_this, _desired);
}

csp_weak_ptr csp_atomic_weak_ptr_exchange(csp_atomic_weak_ptr *const _this, csp_weak_ptr _desired)
{
    assert(_this);

    mtx_t _mtx;

    if (mtx_init(&_mtx, mtx_plain) != thrd_success)
    {
        // TODO: error handling
    }

    if (mtx_lock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    csp_weak_ptr_swap(&_this->_p, &_desired);

    if (mtx_unlock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    mtx_destroy(&_mtx);

    return _desired;
}

csp_weak_ptr csp_atomic_weak_ptr_exchange_explicit(csp_atomic_weak_ptr *const _this, const csp_weak_ptr _desired, const memory_order _order)
{
    assert(_this);

    return csp_atomic_weak_ptr_exchange(_this, _desired);
}

bool csp_atomic_weak_ptr_compare_exchange_weak(csp_atomic_weak_ptr *const _this, csp_weak_ptr *const _expected, const csp_weak_ptr _desired)
{
    assert(_this);

    return csp_atomic_weak_ptr_compare_exchange_strong(_this, _expected, _desired);
}

bool csp_atomic_weak_ptr_compare_exchange_weak_explicit(csp_atomic_weak_ptr *const _this, csp_weak_ptr *const _expected, const csp_weak_ptr _desired, const memory_order _success, const memory_order _failure)
{
    assert(_this);
    assert(_failure != memory_order_release && _failure != memory_order_acq_rel);

    return csp_atomic_weak_ptr_compare_exchange_weak(_this, _expected, _desired);
}

bool csp_atomic_weak_ptr_compare_exchange_strong(csp_atomic_weak_ptr *const _this, csp_weak_ptr *const _expected, const csp_weak_ptr _desired)
{
    assert(_this);

    csp_weak_ptr _tmp;

    mtx_t _mtx;

    if (mtx_init(&_mtx, mtx_plain) != thrd_success)
    {
        // TODO: error handling
    }

    if (mtx_lock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    if (csp_weak_ptr_owner_equals_w(&_this->_p, _expected))
    {
        csp_weak_ptr_swap(&_tmp, &_this->_p);
        csp_weak_ptr_init_w_copy(&_this->_p, &_desired);

        if (mtx_unlock(&_mtx) != thrd_success)
        {
            // TODO: error handling
        }

        mtx_destroy(&_mtx);

        csp_weak_ptr_destroy(&_tmp);

        return true;
    }

    csp_weak_ptr_swap(&_tmp, _expected);
    csp_weak_ptr_init_w_copy(_expected, &_this->_p);

    if (mtx_unlock(&_mtx) != thrd_success)
    {
        // TODO: error handling
    }

    mtx_destroy(&_mtx);

    csp_weak_ptr_destroy(&_tmp);

    return false;
}

bool csp_atomic_weak_ptr_compare_exchange_strong_explicit(csp_atomic_weak_ptr *const _this, csp_weak_ptr *const _expected, const csp_weak_ptr _desired, const memory_order _success, const memory_order _failure)
{
    assert(_this);
    assert(_failure != memory_order_release && _failure != memory_order_acq_rel);

    return csp_atomic_weak_ptr_compare_exchange_strong(_this, _expected, _desired);
}

// TODO: https://en.cppreference.com/w/c/thread
void csp_atomic_weak_ptr_wait(const csp_atomic_weak_ptr *const _this, const csp_weak_ptr _old)
{
    assert(_this);
}

void csp_atomic_weak_ptr_wait_explicit(const csp_atomic_weak_ptr *const _this, const csp_weak_ptr _old, const memory_order _order)
{
    assert(_this);
    assert(_order != memory_order_release && _order != memory_order_acq_rel);

    csp_atomic_weak_ptr_wait(_this, _old);
}

void csp_atomic_weak_ptr_notify_one(csp_atomic_weak_ptr *const _this)
{
    assert(_this);

    csp_atomic_weak_ptr_notify_all(_this);
}

// TODO: https://en.cppreference.com/w/c/thread
void csp_atomic_weak_ptr_notify_all(csp_atomic_weak_ptr *const _this)
{
    assert(_this);
}
