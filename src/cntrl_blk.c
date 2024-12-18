#include "cntrl_blk.h"

#include <assert.h>
#include <stdlib.h>

static inline long csp_cntrl_blk_load(const CSP_CNTRL_BLK_LONG *_value);

static inline long csp_cntrl_blk_load_relaxed(const CSP_CNTRL_BLK_LONG *_value);

static inline long csp_cntrl_blk_load_aquire(const CSP_CNTRL_BLK_LONG *_value);

static inline long csp_cntrl_blk_add_fetch_relaxed(CSP_CNTRL_BLK_LONG *_value);

static inline long csp_cntrl_blk_sub_fetch_relaxed(CSP_CNTRL_BLK_LONG *_value);

static inline bool csp_cntrl_blk_compare_exchange_weak(CSP_CNTRL_BLK_LONG *_value, long *_expected, long _desired);

csp_cntrl_blk *csp_cntrl_blk_init(csp_cntrl_blk *const _this, csp_cntrl_blk_T *const _p, const csp_cntrl_blk_D _d)
{
    assert(_this);

    _this->_p = _p;
    _this->_d = _d;
    _this->_shared_owners = 0;
    _this->_weak_owners = 0;

    return _this;
}

csp_cntrl_blk_D *csp_cntrl_blk_get_deleter(csp_cntrl_blk *const _this)
{
    assert(_this);

    return &_this->_d;
}

long csp_cntrl_blk_use_count(const csp_cntrl_blk *const _this)
{
    assert(_this);

    return csp_cntrl_blk_load_relaxed(&_this->_shared_owners) + 1;
}

csp_cntrl_blk *csp_cntrl_blk_lock(csp_cntrl_blk *const _this)
{
    assert(_this);

    auto _shared_owners = csp_cntrl_blk_load(&_this->_shared_owners);

    while (_shared_owners != -1)
    {
        if (csp_cntrl_blk_compare_exchange_weak(&_this->_shared_owners, &_shared_owners, _shared_owners + 1))
        {
            return _this;
        }
    }

    return nullptr;
}

void csp_cntrl_blk_add_shared(csp_cntrl_blk *const _this)
{
    assert(_this);

    csp_cntrl_blk_add_fetch_relaxed(&_this->_shared_owners);
}

void csp_cntrl_blk_release_shared(csp_cntrl_blk *const _this)
{
    assert(_this);
    assert(_this->_d);

    if (csp_cntrl_blk_sub_fetch_relaxed(&_this->_shared_owners) == -1)
    {
        _this->_d(_this->_p);

        csp_cntrl_blk_release_weak(_this);
    }
}

void csp_cntrl_blk_add_weak(csp_cntrl_blk *const _this)
{
    assert(_this);

    csp_cntrl_blk_add_fetch_relaxed(&_this->_weak_owners);
}

void csp_cntrl_blk_release_weak(csp_cntrl_blk *const _this)
{
    assert(_this);

    if (csp_cntrl_blk_load_aquire(&_this->_weak_owners) == 0 || csp_cntrl_blk_sub_fetch_relaxed(&_this->_weak_owners) == -1)
    {
        free(_this);
    }
}

long csp_cntrl_blk_load(const CSP_CNTRL_BLK_LONG *const _value)
{
#ifdef __STD_NO_ATOMICS__
    return *_value;
#else
    return atomic_load(_value);
#endif
}

long csp_cntrl_blk_load_relaxed(const CSP_CNTRL_BLK_LONG *const _value)
{
#ifdef __STD_NO_ATOMICS__
    return *_value;
#else
    return atomic_load_explicit(_value, memory_order_relaxed);
#endif
}

long csp_cntrl_blk_load_aquire(const CSP_CNTRL_BLK_LONG *const _value)
{
#ifdef __STD_NO_ATOMICS__
    return *_value;
#else
    return atomic_load_explicit(_value, memory_order_acquire);
#endif
}

long csp_cntrl_blk_add_fetch_relaxed(CSP_CNTRL_BLK_LONG *const _value)
{
#ifdef __STD_NO_ATOMICS__
    return ++(*_value);
#else
    return atomic_fetch_add_explicit(_value, 1, memory_order_relaxed) + 1;
#endif
}

long csp_cntrl_blk_sub_fetch_relaxed(CSP_CNTRL_BLK_LONG *const _value)
{
#ifdef __STD_NO_ATOMICS__
    return --(*_value);
#else
    return atomic_fetch_sub_explicit(_value, 1, memory_order_relaxed) + 1;
#endif
}

bool csp_cntrl_blk_compare_exchange_weak(CSP_CNTRL_BLK_LONG *const _value, long *const _expected, const long _desired)
{
#ifdef __STD_NO_ATOMICS__
    if (*_value == *_expected)
    {
        *_value = _desired;

        return true;
    }

    *_expected = *_value;

    return false;
#else
    return atomic_compare_exchange_weak(_value, _expected, _desired);
#endif
}
