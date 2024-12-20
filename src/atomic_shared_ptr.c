#include "csp/atomic_shared_ptr.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <threads.h>

#include "mtx_pool.h"

csp_atomic_shared_ptr csp_atomic_shared_ptr_init(void)
{
    const csp_atomic_shared_ptr _this = { ._r = csp_shared_ptr_init() };

    return _this;
}

csp_atomic_shared_ptr csp_atomic_shared_ptr_init_s(const csp_shared_ptr _desired)
{
    const csp_atomic_shared_ptr _this = { ._r = csp_shared_ptr_init_copy_s(&_desired) };

    return _this;
}

void csp_atomic_shared_ptr_destroy(csp_atomic_shared_ptr* const _this)
{
    assert(_this);

    csp_shared_ptr_destroy(&_this->_r);
}

bool csp_atomic_shared_ptr_is_lock_free(const csp_atomic_shared_ptr* const _this)
{
    assert(_this);

    return false;
}

csp_shared_ptr csp_atomic_shared_ptr_load(const csp_atomic_shared_ptr* const _this)
{
    return csp_atomic_shared_ptr_load_explicit(_this, memory_order_seq_cst);
}

csp_shared_ptr csp_atomic_shared_ptr_load_explicit(const csp_atomic_shared_ptr* const _this, [[maybe_unused]] const memory_order _order)
{
    assert(_this);
    assert(_order != memory_order_release && _order != memory_order_acq_rel);

    auto _mutex = csp_mtx_pool_get(_this);

    mtx_lock(_mutex);

    const auto _r = csp_shared_ptr_init_copy_s(&_this->_r);

    mtx_unlock(_mutex);

    return _r;
}

void csp_atomic_shared_ptr_store(csp_atomic_shared_ptr* const _this, csp_shared_ptr _desired)
{
    csp_atomic_shared_ptr_store_explicit(_this, _desired, memory_order_seq_cst);
}

void csp_atomic_shared_ptr_store_explicit(csp_atomic_shared_ptr* const _this, const csp_shared_ptr _desired, [[maybe_unused]] const memory_order _order)
{
    assert(_this);
    assert(_order != memory_order_consume && _order != memory_order_acquire && _order != memory_order_acq_rel);

    auto _mutex = csp_mtx_pool_get(_this);

    mtx_lock(_mutex);

    csp_shared_ptr_swap(&_this->_r, &_desired);

    mtx_unlock(_mutex);
}

csp_shared_ptr csp_atomic_shared_ptr_exchange(csp_atomic_shared_ptr* const _this, csp_shared_ptr _desired)
{
    return csp_atomic_shared_ptr_exchange_explicit(_this, _desired, memory_order_seq_cst);
}

csp_shared_ptr csp_atomic_shared_ptr_exchange_explicit(csp_atomic_shared_ptr* const _this, const csp_shared_ptr _desired, [[maybe_unused]] const memory_order _order)
{
    assert(_this);

    auto _mutex = csp_mtx_pool_get(_this);

    mtx_lock(_mutex);

    csp_shared_ptr_swap(&_this->_r, &_desired);

    mtx_unlock(_mutex);

    return _desired;
}

bool csp_atomic_shared_ptr_compare_exchange_weak(csp_atomic_shared_ptr* const _this, csp_shared_ptr* const _expected, const csp_shared_ptr _desired)
{
    return csp_atomic_shared_ptr_compare_exchange_weak_explicit(_this, _expected, _desired, memory_order_seq_cst, memory_order_seq_cst);
}

bool csp_atomic_shared_ptr_compare_exchange_weak_explicit(csp_atomic_shared_ptr* const _this, csp_shared_ptr* const _expected, const csp_shared_ptr _desired, [[maybe_unused]] const memory_order _success, [[maybe_unused]] const memory_order _failure)
{
    assert(_this);
    assert(_failure != memory_order_release && _failure != memory_order_acq_rel);

    return csp_atomic_shared_ptr_compare_exchange_strong_explicit(_this, _expected, _desired, _success, _failure);
}

bool csp_atomic_shared_ptr_compare_exchange_strong(csp_atomic_shared_ptr* const _this, csp_shared_ptr* const _expected, const csp_shared_ptr _desired)
{
    return csp_atomic_shared_ptr_compare_exchange_strong_explicit(_this, _expected, _desired, memory_order_seq_cst, memory_order_seq_cst);
}

bool csp_atomic_shared_ptr_compare_exchange_strong_explicit(csp_atomic_shared_ptr* const _this, csp_shared_ptr* const _expected, const csp_shared_ptr _desired, [[maybe_unused]] const memory_order _success, [[maybe_unused]] const memory_order _failure)
{
    assert(_this);
    assert(_failure != memory_order_release && _failure != memory_order_acq_rel);

    auto _mutex = csp_mtx_pool_get(_this);

    mtx_lock(_mutex);

    if (csp_shared_ptr_owner_equal_s(&_this->_r, _expected))
    {
        auto _tmp = csp_shared_ptr_init();
        csp_shared_ptr_swap(&_tmp, &_this->_r);
        csp_shared_ptr_copy_s(&_this->_r, &_desired);

        mtx_unlock(_mutex);

        csp_shared_ptr_destroy(&_tmp);

        return true;
    }

    auto _tmp = csp_shared_ptr_init();
    csp_shared_ptr_swap(&_tmp, _expected);
    csp_shared_ptr_copy_s(_expected, &_this->_r);

    mtx_unlock(_mutex);

    csp_shared_ptr_destroy(&_tmp);

    return false;
}

void csp_atomic_shared_ptr_wait(const csp_atomic_shared_ptr* const _this, const csp_shared_ptr* const _old)
{
    csp_atomic_shared_ptr_wait_explicit(_this, _old, memory_order_seq_cst);
}

void csp_atomic_shared_ptr_wait_explicit(const csp_atomic_shared_ptr* const _this, const csp_shared_ptr* const _old, const memory_order _order)
{
    assert(_this);
    assert(_old);
    assert(_order != memory_order_release && _order != memory_order_acq_rel);

    while (true)
    {
        const auto _r = csp_atomic_shared_ptr_load_explicit(_this, _order);

        if (memcmp(&_r, _old, sizeof(_old)) != 0)
        {
            break;
        }

        thrd_yield();
    }
}

void csp_atomic_shared_ptr_notify_one(const csp_atomic_shared_ptr* const _this)
{
    assert(_this);

    csp_atomic_shared_ptr_notify_all(_this);
}

void csp_atomic_shared_ptr_notify_all(const csp_atomic_shared_ptr* const _this)
{
    assert(_this);

    auto _mutex = csp_mtx_pool_get(_this);

    mtx_unlock(_mutex);
}
