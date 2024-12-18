#include "csp/atomic_weak_ptr.h"

#include <assert.h>
#include <stddef.h>
#include <threads.h>

#include "mtx_pool.h"

csp_atomic_weak_ptr csp_atomic_weak_ptr_init(void)
{
    const csp_atomic_weak_ptr _this = { ._r = csp_weak_ptr_init() };

    return _this;
}

csp_atomic_weak_ptr csp_atomic_weak_ptr_init_s(const csp_weak_ptr _desired)
{
    const csp_atomic_weak_ptr _this = { ._r = csp_weak_ptr_init_copy_w(&_desired) };

    return _this;
}

void csp_atomic_weak_ptr_destroy(csp_atomic_weak_ptr* const _this)
{
    assert(_this);

    csp_weak_ptr_destroy(&_this->_r);
}

void csp_atomic_weak_ptr_s(csp_atomic_weak_ptr* const _this, const csp_weak_ptr _desired, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    csp_atomic_weak_ptr_store(_this, _desired, _e);
}

bool csp_atomic_weak_ptr_is_lock_free(const csp_atomic_weak_ptr* const _this)
{
    assert(_this);

    return false;
}

csp_weak_ptr csp_atomic_weak_ptr_load(const csp_atomic_weak_ptr* const _this, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    auto _mutex = csp_mtx_pool_get(_this, _e);
    if (*_e != CSP_SUCCESS)
    {
        return (csp_weak_ptr) { ._p = nullptr, ._cntrl = nullptr };
    }

    if (mtx_lock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return (csp_weak_ptr) { ._p = nullptr, ._cntrl = nullptr };
    }

    const auto _r = csp_weak_ptr_init_copy_w(&_this->_r);

    if (mtx_unlock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return (csp_weak_ptr) { ._p = nullptr, ._cntrl = nullptr };
    }

    *_e = CSP_SUCCESS;

    return _r;
}

csp_weak_ptr csp_atomic_weak_ptr_load_explicit(const csp_atomic_weak_ptr* const _this, [[maybe_unused]] const memory_order _order, csp_exception* const _e)
{
    assert(_this);
    assert(_order != memory_order_release && _order != memory_order_acq_rel);
    assert(_e);

    return csp_atomic_weak_ptr_load(_this, _e);
}

void csp_atomic_weak_ptr_store(csp_atomic_weak_ptr* const _this, csp_weak_ptr _desired, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    auto _mutex = csp_mtx_pool_get(_this, _e);
    if (*_e != CSP_SUCCESS)
    {
        return;
    }

    if (mtx_lock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return;
    }

    csp_weak_ptr_swap(&_this->_r, &_desired);

    if (mtx_unlock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return;
    }

    *_e = CSP_SUCCESS;
}

void csp_atomic_weak_ptr_store_explicit(csp_atomic_weak_ptr* const _this, const csp_weak_ptr _desired, [[maybe_unused]] const memory_order _order, csp_exception* const _e)
{
    assert(_this);
    assert(_order != memory_order_consume && _order != memory_order_acquire && _order != memory_order_acq_rel);
    assert(_e);

    csp_atomic_weak_ptr_store(_this, _desired, _e);
}

csp_weak_ptr csp_atomic_weak_ptr_exchange(csp_atomic_weak_ptr* const _this, csp_weak_ptr _desired, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    auto _mutex = csp_mtx_pool_get(_this, _e);
    if (*_e != CSP_SUCCESS)
    {
        return (csp_weak_ptr) { ._p = nullptr, ._cntrl = nullptr };
    }

    if (mtx_lock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return (csp_weak_ptr) { ._p = nullptr, ._cntrl = nullptr };
    }

    csp_weak_ptr_swap(&_this->_r, &_desired);

    if (mtx_unlock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return (csp_weak_ptr) { ._p = nullptr, ._cntrl = nullptr };
    }

    *_e = CSP_SUCCESS;

    return _desired;
}

csp_weak_ptr csp_atomic_weak_ptr_exchange_explicit(csp_atomic_weak_ptr* const _this, const csp_weak_ptr _desired, [[maybe_unused]] const memory_order _order, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    return csp_atomic_weak_ptr_exchange(_this, _desired, _e);
}

bool csp_atomic_weak_ptr_compare_exchange_weak(csp_atomic_weak_ptr* const _this, csp_weak_ptr* const _expected, const csp_weak_ptr _desired, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    return csp_atomic_weak_ptr_compare_exchange_strong(_this, _expected, _desired, _e);
}

bool csp_atomic_weak_ptr_compare_exchange_weak_explicit(csp_atomic_weak_ptr* const _this, csp_weak_ptr* const _expected, const csp_weak_ptr _desired, [[maybe_unused]] const memory_order _success, [[maybe_unused]] const memory_order _failure, csp_exception* const _e)
{
    assert(_this);
    assert(_failure != memory_order_release && _failure != memory_order_acq_rel);
    assert(_e);

    return csp_atomic_weak_ptr_compare_exchange_weak(_this, _expected, _desired, _e);
}

bool csp_atomic_weak_ptr_compare_exchange_strong(csp_atomic_weak_ptr* const _this, csp_weak_ptr* const _expected, const csp_weak_ptr _desired, csp_exception* const _e)
{
    assert(_this);
    assert(_e);

    auto _mutex = csp_mtx_pool_get(_this, _e);
    if (*_e != CSP_SUCCESS)
    {
        return false;
    }

    if (mtx_lock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return false;
    }

    if (csp_weak_ptr_owner_equals_w(&_this->_r, _expected))
    {
        auto _tmp = csp_weak_ptr_init();
        csp_weak_ptr_swap(&_tmp, &_this->_r);
        csp_weak_ptr_copy_w(&_this->_r, &_desired);

        if (mtx_unlock(_mutex) != thrd_success)
        {
            *_e = CSP_BAD_ATOMIC;

            return false;
        }

        csp_weak_ptr_destroy(&_tmp);

        *_e = CSP_SUCCESS;

        return true;
    }

    auto _tmp = csp_weak_ptr_init();
    csp_weak_ptr_swap(&_tmp, _expected);
    csp_weak_ptr_copy_w(_expected, &_this->_r);

    if (mtx_unlock(_mutex) != thrd_success)
    {
        *_e = CSP_BAD_ATOMIC;

        return false;
    }

    csp_weak_ptr_destroy(&_tmp);

    *_e = CSP_SUCCESS;

    return false;
}

bool csp_atomic_weak_ptr_compare_exchange_strong_explicit(csp_atomic_weak_ptr* const _this, csp_weak_ptr* const _expected, const csp_weak_ptr _desired, [[maybe_unused]] const memory_order _success, [[maybe_unused]] const memory_order _failure, csp_exception* const _e)
{
    assert(_this);
    assert(_failure != memory_order_release && _failure != memory_order_acq_rel);
    assert(_e);

    return csp_atomic_weak_ptr_compare_exchange_strong(_this, _expected, _desired, _e);
}
