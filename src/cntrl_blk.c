#include "cntrl_blk.h"

#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>

struct csp_cntrl_blk
{
    csp_cntrl_blk_T *_p;
    csp_cntrl_blk_D _d;
#ifndef CSP_NO_SYNCHRONIZATION
    atomic_long _shared_owners;
    atomic_long _weak_owners;
#else
    long _shared_owners;
    long _weak_owners;
#endif
};

void csp_cntrl_blk_init(csp_cntrl_blk *const _this, csp_cntrl_blk_T *const _p, const csp_cntrl_blk_D _d)
{
    assert(_this);

    _this->_p = _p;
    _this->_d = _d;
    _this->_shared_owners = 0;
    _this->_weak_owners = 0;
}

csp_cntrl_blk_D *csp_cntrl_blk_get_deleter(csp_cntrl_blk *const _this)
{
    assert(_this);

    return &_this->_d;
}

long csp_cntrl_blk_use_count(const csp_cntrl_blk *const _this)
{
    assert(_this);

#ifndef CSP_NO_SYNCHRONIZATION
    const long _shared_owners = atomic_load_explicit(&_this->_shared_owners, memory_order_relaxed);
#else
    const long _shared_owners = _this->_shared_owners;
#endif

    return _shared_owners + 1;
}

csp_cntrl_blk *csp_cntrl_blk_lock(csp_cntrl_blk *const _this)
{
    assert(_this);

#ifndef CSP_NO_SYNCHRONIZATION
    long _shared_owners = atomic_load(&_this->_shared_owners);
#else
    long _shared_owners = _this->_shared_owners;
#endif

    while (_shared_owners != -1)
    {
#ifndef CSP_NO_SYNCHRONIZATION
    const bool equal = atomic_compare_exchange_strong(&_this->_shared_owners, &_shared_owners, _shared_owners + 1);
#else
    bool equal = false;
    if (_this->_shared_owners == _shared_owners) {
        _this->_shared_owners = __after;
        
        equal = true;
    }
#endif

        if (equal)
        {
            return _this;
        }
    }

    return NULL;
}

void csp_cntrl_blk_add_shared(csp_cntrl_blk *const _this)
{
    assert(_this);

#ifndef CSP_NO_SYNCHRONIZATION
    atomic_fetch_add_explicit(&_this->_shared_owners, 1, memory_order_relaxed);
#else
    ++_this->_shared_owners;
#endif
}

void csp_cntrl_blk_release_shared(csp_cntrl_blk *const _this)
{
    assert(_this);

#ifndef CSP_NO_SYNCHRONIZATION
    atomic_fetch_sub_explicit(&_this->_shared_owners, 1, memory_order_acq_rel);
#else
    --_this->_shared_owners;
#endif

    if (_this->_shared_owners == -1)
    {
        assert(_this->_d);

        _this->_d(_this->_p);

        csp_cntrl_blk_release_weak(_this);
    }
}

void csp_cntrl_blk_add_weak(csp_cntrl_blk *const _this)
{
    assert(_this);

#ifndef CSP_NO_SYNCHRONIZATION
    atomic_fetch_add_explicit(&_this->_weak_owners, 1, memory_order_relaxed);
#else
    ++_this->_weak_owners;
#endif
}

void csp_cntrl_blk_release_weak(csp_cntrl_blk *const _this)
{
    assert(_this);

#ifndef CSP_NO_SYNCHRONIZATION
    atomic_fetch_sub_explicit(&_this->_weak_owners, 1, memory_order_acq_rel);
#else
    --_this->_weak_owners;
#endif

    if (_this->_weak_owners == -1)
    {
        free(_this);
    }
}
