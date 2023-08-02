#include "csp/weak_ptr.h"

#include <assert.h>

#include "csp/shared_ptr.h"

#include "cntrl_blk.h"

csp_weak_ptr csp_weak_ptr_init(void)
{
    const csp_weak_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

    assert(csp_weak_ptr_use_count(&_this) == 0);

    return _this;
}

csp_weak_ptr csp_weak_ptr_init_w_copy(const csp_weak_ptr *const _r)
{
    assert(_r);

    const csp_weak_ptr _this = { ._p = _r->_p, ._cntrl = _r->_cntrl };

    if (_this._cntrl)
    {
        csp_cntrl_blk_add_weak(_this._cntrl);
    }

    assert(csp_weak_ptr_use_count(&_this) == csp_weak_ptr_use_count(_r));

    return _this;
}

csp_weak_ptr csp_weak_ptr_init_w_move(csp_weak_ptr *const _r)
{
    assert(_r);

    [[maybe_unused]] const auto _shared_owners = csp_weak_ptr_use_count(_r);

    const csp_weak_ptr _this = { ._p = _r->_p, ._cntrl = _r->_cntrl };

    _r->_p = nullptr;
    _r->_cntrl = nullptr;

    assert(csp_weak_ptr_use_count(&_this) == _shared_owners);
    assert(csp_weak_ptr_use_count(_r) == 0);

    return _this;
}

csp_weak_ptr csp_weak_ptr_init_w_p_copy(const csp_weak_ptr *const _r, csp_weak_ptr_T *const _p)
{
    assert(_r);

    const csp_weak_ptr _this = { ._p = _p, ._cntrl = _r->_cntrl };

    if (_this._cntrl)
    {
        csp_cntrl_blk_add_weak(_this._cntrl);
    }

    assert(csp_weak_ptr_use_count(&_this) == csp_weak_ptr_use_count(_r));

    return _this;
}

csp_weak_ptr csp_weak_ptr_init_w_p_move(csp_weak_ptr *const _r, csp_weak_ptr_T *const _p)
{
    assert(_r);

    [[maybe_unused]] const auto _shared_owners = csp_weak_ptr_use_count(_r);

    const csp_weak_ptr _this = { ._p = _p, ._cntrl = _r->_cntrl };

    _r->_p = nullptr;
    _r->_cntrl = nullptr;

    assert(csp_weak_ptr_use_count(&_this) == _shared_owners);
    assert(csp_weak_ptr_use_count(_r) == 0);

    return _this;
}

csp_weak_ptr csp_weak_ptr_init_s_copy(const csp_shared_ptr *const _r)
{
    assert(_r);

    const csp_weak_ptr _this = { ._p = _r->_p, ._cntrl = _r->_cntrl };

    if (_this._cntrl)
    {
        csp_cntrl_blk_add_weak(_this._cntrl);
    }

    assert(csp_weak_ptr_use_count(&_this) == csp_shared_ptr_use_count(_r));

    return _this;
}

csp_weak_ptr csp_weak_ptr_init_s_p_copy(const csp_shared_ptr *const _r, csp_weak_ptr_T *const _p)
{
    assert(_r);

    const csp_weak_ptr _this = { ._p = _p, ._cntrl = _r->_cntrl };

    if (_this._cntrl)
    {
        csp_cntrl_blk_add_weak(_this._cntrl);
    }

    assert(csp_weak_ptr_use_count(&_this) == csp_shared_ptr_use_count(_r));

    return _this;
}

void csp_weak_ptr_destroy(csp_weak_ptr *const _this)
{
    assert(_this);

    if (_this->_cntrl)
    {
        csp_cntrl_blk_release_weak(_this->_cntrl);
    }
}

csp_weak_ptr *csp_weak_ptr_copy_w(csp_weak_ptr *_this, const csp_weak_ptr *_r)
{
    assert(_this);
    assert(_r);

    csp_weak_ptr _tmp = csp_weak_ptr_init_w_copy(_r);
    csp_weak_ptr_swap(&_tmp, _this);
    csp_weak_ptr_destroy(&_tmp);

    return _this;
}

csp_weak_ptr *csp_weak_ptr_move_w(csp_weak_ptr *_this, csp_weak_ptr *_r)
{
    assert(_this);
    assert(_r);

    csp_weak_ptr _tmp = csp_weak_ptr_init_w_move(_r);
    csp_weak_ptr_swap(&_tmp, _this);
    csp_weak_ptr_destroy(&_tmp);

    return _this;
}

csp_weak_ptr *csp_weak_ptr_copy_s(csp_weak_ptr *_this, const csp_shared_ptr *_r)
{
    assert(_this);
    assert(_r);

    csp_weak_ptr _tmp = csp_weak_ptr_init_s_copy(_r);
    csp_weak_ptr_swap(&_tmp, _this);
    csp_weak_ptr_destroy(&_tmp);

    return _this;
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

    csp_shared_ptr _r = { ._p = nullptr, ._cntrl = _this->_cntrl ? csp_cntrl_blk_lock(_this->_cntrl) : _this->_cntrl };

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

    csp_weak_ptr _tmp = csp_weak_ptr_init();
    csp_weak_ptr_swap(&_tmp, _this);
    csp_weak_ptr_destroy(&_tmp);
}

void csp_weak_ptr_swap(csp_weak_ptr *const _this, csp_weak_ptr *const _r)
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

void csp_swap_w(csp_weak_ptr *const _x, csp_weak_ptr *const _y)
{
    assert(_x);
    assert(_y);

    csp_weak_ptr_swap(_x, _y);
}
