#include "csp/weak_ptr.h"

#include <assert.h>

#include "csp/shared_ptr.h"

#include "cntrl_blk.h"

void csp_weak_ptr_init(csp_weak_ptr *const _this)
{
    assert(_this);

    _this->_p = NULL;
    _this->_cntrl = NULL;

    assert(csp_weak_ptr_use_count(_this) == 0);
}

void csp_weak_ptr_init_w_copy(csp_weak_ptr *const _this, const csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    _this->_p = _r->_p;
    _this->_cntrl = _r->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_weak(_this->_cntrl);
    }

    assert(csp_weak_ptr_use_count(_this) == csp_weak_ptr_use_count(_r));
}

void csp_weak_ptr_init_w_move(csp_weak_ptr *const _this, csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    _this->_p = _r->_p;
    _this->_cntrl = _r->_cntrl;

    _r->_p = NULL;
    _r->_cntrl = NULL;

    assert(csp_weak_ptr_use_count(_r) == 0);
}

void csp_weak_ptr_init_w_p_copy(csp_weak_ptr *const _this, const csp_weak_ptr *const _r, csp_weak_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    _this->_p = _p;
    _this->_cntrl = _r->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_weak(_this->_cntrl);
    }

    assert(csp_weak_ptr_use_count(_this) == csp_weak_ptr_use_count(_r));
}

void csp_weak_ptr_init_w_p_move(csp_weak_ptr *const _this, csp_weak_ptr *const _r, csp_weak_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    _this->_p = _p;
    _this->_cntrl = _r->_cntrl;

    _r->_p = NULL;
    _r->_cntrl = NULL;

    assert(csp_weak_ptr_use_count(_r) == 0);
}

void csp_weak_ptr_init_s_copy(csp_weak_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    _this->_p = _r->_p;
    _this->_cntrl = _r->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_weak(_this->_cntrl);
    }

    assert(csp_weak_ptr_use_count(_this) == csp_shared_ptr_use_count(_r));
}

void csp_weak_ptr_init_s_p_copy(csp_weak_ptr *const _this, const csp_shared_ptr *const _r, csp_weak_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    _this->_p = _p;
    _this->_cntrl = _r->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_weak(_this->_cntrl);
    }

    assert(csp_weak_ptr_use_count(_this) == csp_shared_ptr_use_count(_r));
}

void csp_weak_ptr_destroy(csp_weak_ptr *const _this)
{
    assert(_this);

    if (_this->_cntrl)
    {
        csp_cntrl_blk_release_weak(_this->_cntrl);
    }
}

long csp_weak_ptr_use_count(const csp_weak_ptr *const _this)
{
    assert(_this);

    return _this->_cntrl ? csp_cntrl_blk_use_count(_this->_cntrl) : 0;
}

bool csp_weak_ptr_expired(const csp_weak_ptr *const _this)
{
    assert(_this);

    return csp_weak_ptr_use_count(_this) == 0;
}

csp_shared_ptr csp_weak_ptr_lock(const csp_weak_ptr *const _this)
{
    assert(_this);

    csp_shared_ptr _r;

    _r._p = NULL;
    _r._cntrl = _this->_cntrl ? csp_cntrl_blk_lock(_this->_cntrl) : _this->_cntrl;

    if (_r._cntrl)
    {
        _r._p = _this->_p;
    }

    return _r;
}

bool csp_weak_ptr_owner_before_s(const csp_weak_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl < _r->_cntrl;
}

bool csp_weak_ptr_owner_before_w(const csp_weak_ptr *const _this, const csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl < _r->_cntrl;
}

bool csp_weak_ptr_owner_equals_s(const csp_weak_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl == _r->_cntrl;
}

bool csp_weak_ptr_owner_equals_w(const csp_weak_ptr *const _this, const csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl == _r->_cntrl;
}

size_t csp_weak_ptr_owner_hash_value(const csp_weak_ptr *const _this)
{
    assert(_this);

    return _this->_cntrl ? (size_t)_this->_cntrl : 0;
}

void csp_weak_ptr_reset(csp_weak_ptr *const _this)
{
    assert(_this);

    csp_weak_ptr _tmp;

    csp_weak_ptr_init(&_tmp);

    csp_weak_ptr_swap(&_tmp, _this);

    csp_weak_ptr_destroy(&_tmp);
}

void csp_weak_ptr_swap(csp_weak_ptr *const _this, csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    csp_weak_ptr_T *const _p = _this->_p;
    _this->_p = _r->_p;
    _r->_p = _p;

    csp_cntrl_blk *const _cntrl = _this->_cntrl;
    _this->_cntrl = _r->_cntrl;
    _r->_cntrl = _cntrl;
}
