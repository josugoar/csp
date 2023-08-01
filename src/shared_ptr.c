#include "csp/shared_ptr.h"

#include <assert.h>
#include <stdlib.h>

#include "csp/unique_ptr.h"
#include "csp/weak_ptr.h"

#include "cntrl_blk.h"

void csp_shared_ptr_init(csp_shared_ptr *const _this)
{
    assert(_this);

    _this->_p = nullptr;
    _this->_cntrl = nullptr;

    assert(!csp_shared_ptr_get(_this));
    assert(csp_shared_ptr_use_count(_this) == 0);
}

void csp_shared_ptr_init_p(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p)
{
    assert(_this);

    const auto _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_this->_cntrl));
    if (!_cntrl)
    {
        csp_default_delete(_p);

        // TODO: error handling
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    csp_cntrl_blk_init(_cntrl, _p, csp_default_delete);

    _this->_p = _p;
    _this->_cntrl = _cntrl;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(csp_shared_ptr_use_count(_this) == 1);
}

void csp_shared_ptr_init_pd(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p, const csp_shared_ptr_D _d)
{
    assert(_this);

    const auto _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_this->_cntrl));
    if (!_cntrl)
    {
        assert(_d);

        _d(_p);

        // TODO: error handling
        _this->_p = nullptr;
        _this->_cntrl = nullptr;
    }

    csp_cntrl_blk_init(_cntrl, _p, _d);
    
    _this->_p = _p;
    _this->_cntrl = _cntrl;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(csp_shared_ptr_use_count(_this) == 1);
}

void csp_shared_ptr_init_s_copy(csp_shared_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    _this->_p = _r->_p;
    _this->_cntrl = _r->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_shared(_this->_cntrl);
    }

    assert(csp_shared_ptr_get(_this) == csp_shared_ptr_get(_r));
    assert(csp_shared_ptr_use_count(_this) == csp_shared_ptr_use_count(_r));
}

void csp_shared_ptr_init_s_move(csp_shared_ptr *const _this, csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    [[maybe_unused]] const auto _p = csp_shared_ptr_get(_r);
    [[maybe_unused]] const auto _shared_owners = csp_shared_ptr_use_count(_r);

    _this->_p = _r->_p;
    _this->_cntrl = _r->_cntrl;

    _r->_p = nullptr;
    _r->_cntrl = nullptr;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(!csp_shared_ptr_get(_r));
    assert(csp_shared_ptr_use_count(_this) == _shared_owners);
}

void csp_shared_ptr_init_s_p_copy(csp_shared_ptr *const _this, const csp_shared_ptr *const _r, csp_shared_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    _this->_p = _p;
    _this->_cntrl = _r->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_shared(_this->_cntrl);
    }

    assert(csp_shared_ptr_get(_this) == _p);
    assert(csp_shared_ptr_use_count(_this) == csp_shared_ptr_use_count(_r));
}

void csp_shared_ptr_init_s_p_move(csp_shared_ptr *const _this, csp_shared_ptr *const _r, csp_shared_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    [[maybe_unused]] const auto _shared_owners = csp_shared_ptr_use_count(_r);

    _this->_p = _p;
    _this->_cntrl = _r->_cntrl;

    _r->_p = nullptr;
    _r->_cntrl = nullptr;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(!csp_shared_ptr_get(_r));
    assert(csp_shared_ptr_use_count(_this) == _shared_owners);
}

void csp_shared_ptr_init_w_copy(csp_shared_ptr *const _this, const csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    const auto _cntrl = _r->_cntrl ? csp_cntrl_blk_lock(_r->_cntrl) : _r->_cntrl;
    if (!_cntrl)
    {
        // TODO: error handling
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    _this->_p = _r->_p;
    _this->_cntrl = _cntrl;

    assert(csp_shared_ptr_use_count(_this) == csp_weak_ptr_use_count(_r));
}

void csp_shared_ptr_init_u_move(csp_shared_ptr *const _this, csp_unique_ptr *const _r)
{
    assert(_this);
    assert(_r);

    const auto _p = csp_unique_ptr_get(_r);

    if (!_r->_p)
    {
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    const auto _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_this->_cntrl));
    if (!_cntrl)
    {
        // TODO: error handling
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    csp_cntrl_blk_init(_cntrl, _r->_p, _r->_d);

    _this->_p = _r->_p;
    _this->_cntrl = _cntrl;

    _r->_p = nullptr;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(csp_shared_ptr_use_count(_this) == 1);
}

void csp_shared_ptr_destroy(csp_shared_ptr *const _this)
{
    assert(_this);

    if (_this->_cntrl)
    {
        csp_cntrl_blk_release_shared(_this->_cntrl);
    }
}

csp_shared_ptr_T *csp_shared_ptr_get(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_p;
}

csp_shared_ptr_D *csp_shared_ptr_get_deleter(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_cntrl ? csp_cntrl_blk_get_deleter(_this->_cntrl) : nullptr;
}

long csp_shared_ptr_use_count(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_cntrl ? csp_cntrl_blk_use_count(_this->_cntrl) : 0;
}

bool csp_shared_ptr_owner_before_s(const csp_shared_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl < _r->_cntrl;
}

bool csp_shared_ptr_owner_before_w(const csp_shared_ptr *const _this, const csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl < _r->_cntrl;
}

bool csp_shared_ptr_owner_equals_s(const csp_shared_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl == _r->_cntrl;
}

bool csp_shared_ptr_owner_equals_w(const csp_shared_ptr *const _this, const csp_weak_ptr *const _r)
{
    assert(_this);
    assert(_r);

    return _this->_cntrl == _r->_cntrl;
}

size_t csp_shared_ptr_owner_hash_value(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_cntrl ? (size_t)_this->_cntrl : 0;
}

size_t csp_shared_ptr_hash(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_p ? (size_t)_this->_p : 0;
}

void csp_shared_ptr_reset(csp_shared_ptr *const _this)
{
    assert(_this);

    csp_shared_ptr _tmp;

    csp_shared_ptr_init(&_tmp);

    csp_shared_ptr_swap(&_tmp, _this);

    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_p(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p)
{
    assert(_this);

    csp_shared_ptr _tmp;

    csp_shared_ptr_init_p(&_tmp, _p);
    // TODO: error handling
    if (!csp_shared_ptr_get(&_tmp))
    {
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);

    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_pd(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p, const csp_shared_ptr_D _d)
{
    assert(_this);

    csp_shared_ptr _tmp;
    
    csp_shared_ptr_init_pd(&_tmp, _p, _d);
    // TODO: error handling
    if (!csp_shared_ptr_get(&_tmp))
    {
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);

    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_s_p_copy(csp_shared_ptr *const _this, const csp_shared_ptr *const _r, csp_shared_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp;

    csp_shared_ptr_init_s_p_copy(&_tmp, _r, _p);
    // TODO: error handling
    if (!csp_shared_ptr_get(&_tmp))
    {
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);

    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_s_p_move(csp_shared_ptr *const _this, csp_shared_ptr *const _r, csp_shared_ptr_T *const _p)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp;

    csp_shared_ptr_init_s_p_move(&_tmp, _r, _p);
    // TODO: error handling
    if (!csp_shared_ptr_get(&_tmp))
    {
        _this->_p = nullptr;
        _this->_cntrl = nullptr;

        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);

    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_swap(csp_shared_ptr *const _this, csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    const auto _p = _this->_p;
    _this->_p = _r->_p;
    _r->_p = _p;

    const auto _cntrl = _this->_cntrl;
    _this->_cntrl = _r->_cntrl;
    _r->_cntrl = _cntrl;
}

csp_shared_ptr csp_make_shared_for_overwrite(const size_t _size)
{
    csp_shared_ptr _r;

    const auto _ptr = (unsigned char *)malloc(_size + sizeof(_r._cntrl));
    if (!_ptr)
    {
        // TODO: error handling
        _r._p = nullptr;
        _r._cntrl = nullptr;

        return _r;
    }

    const auto _p = (csp_shared_ptr_T *)_ptr;
    const auto _cntrl = (csp_cntrl_blk *)(_ptr + _size);

    csp_cntrl_blk_init(_cntrl, _p, csp_default_delete);

    _r._p = _p;
    _r._cntrl = _cntrl;

    return _r;
}
