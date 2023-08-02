#include "csp/shared_ptr.h"

#include <assert.h>
#include <stdlib.h>

#include "csp/exception.h"
#include "csp/unique_ptr.h"
#include "csp/weak_ptr.h"

#include "cntrl_blk.h"

csp_shared_ptr csp_shared_ptr_init(void)
{
    const csp_shared_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

    assert(!csp_shared_ptr_get(&_this));
    assert(csp_shared_ptr_use_count(&_this) == 0);

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_p(csp_shared_ptr_T *const _p, csp_exception *const _exception)
{
    csp_cntrl_blk *const _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_cntrl));
    if (!_cntrl)
    {
        csp_default_delete(_p);

        const csp_shared_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

        *_exception = CSP_BAD_ALLOC;

        return _this;
    }

    csp_cntrl_blk_init(_cntrl, _p, csp_default_delete);
    const csp_shared_ptr _this = { ._p = _p, ._cntrl = _cntrl };

    *_exception = CSP_SUCCESS;

    assert(csp_shared_ptr_get(&_this) == _p);
    assert(csp_shared_ptr_use_count(&_this) == 1);

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_pd(csp_shared_ptr_T *const _p, const csp_shared_ptr_D _d, csp_exception *const _exception)
{
    assert(_d);

    csp_cntrl_blk *const _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_cntrl));
    if (!_cntrl)
    {
        _d(_p);

        const csp_shared_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

        *_exception = CSP_BAD_ALLOC;
    }

    csp_cntrl_blk_init(_cntrl, _p, _d);
    const csp_shared_ptr _this = { ._p = _p, ._cntrl = _cntrl };

    *_exception = CSP_SUCCESS;

    assert(csp_shared_ptr_get(&_this) == _p);
    assert(csp_shared_ptr_use_count(&_this) == 1);

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_s_copy(const csp_shared_ptr *const _r)
{
    assert(_r);

    const csp_shared_ptr _this = { ._p = _r->_p, ._cntrl = _r->_cntrl };

    if (_this._cntrl)
    {
        csp_cntrl_blk_add_shared(_this._cntrl);
    }

    assert(csp_shared_ptr_get(&_this) == csp_shared_ptr_get(_r));
    assert(csp_shared_ptr_use_count(&_this) == csp_shared_ptr_use_count(_r));

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_s_move(csp_shared_ptr *const _r)
{
    assert(_r);

    [[maybe_unused]] const auto _p = csp_shared_ptr_get(_r);
    [[maybe_unused]] const auto _shared_owners = csp_shared_ptr_use_count(_r);

    const csp_shared_ptr _this = { ._p = _r->_p, ._cntrl = _r->_cntrl };

    _r->_p = nullptr;
    _r->_cntrl = nullptr;

    assert(csp_shared_ptr_get(&_this) == _p);
    assert(!csp_shared_ptr_get(_r));
    assert(csp_shared_ptr_use_count(&_this) == _shared_owners);
    assert(csp_shared_ptr_use_count(_r) == 0);

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_s_p_copy(const csp_shared_ptr *const _r, csp_shared_ptr_T *const _p)
{
    assert(_r);

    const csp_shared_ptr _this = { ._p = _p, ._cntrl = _r->_cntrl };

    if (_this._cntrl)
    {
        csp_cntrl_blk_add_shared(_this._cntrl);
    }

    assert(csp_shared_ptr_get(&_this) == _p);
    assert(csp_shared_ptr_use_count(&_this) == csp_shared_ptr_use_count(_r));

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_s_p_move(csp_shared_ptr *const _r, csp_shared_ptr_T *const _p)
{
    assert(_r);

    [[maybe_unused]] const auto _shared_owners = csp_shared_ptr_use_count(_r);

    const csp_shared_ptr _this = { ._p = _p, ._cntrl = _r->_cntrl };

    _r->_p = nullptr;
    _r->_cntrl = nullptr;

    assert(csp_shared_ptr_get(&_this) == _p);
    assert(!csp_shared_ptr_get(_r));
    assert(csp_shared_ptr_use_count(&_this) == _shared_owners);

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_w_copy(const csp_weak_ptr *const _r, csp_exception *const _exception)
{
    assert(_r);

    const auto _cntrl = _r->_cntrl ? csp_cntrl_blk_lock(_r->_cntrl) : _r->_cntrl;
    if (!_cntrl)
    {
        const csp_shared_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

        *_exception = CSP_BAD_WEAK_PTR;

        return _this;
    }

    const csp_shared_ptr _this = { ._p = _r->_p, ._cntrl = _cntrl };

    *_exception = CSP_SUCCESS;

    assert(csp_shared_ptr_use_count(&_this) == csp_weak_ptr_use_count(_r));

    return _this;
}

csp_shared_ptr csp_shared_ptr_init_u_move(csp_unique_ptr *const _r, csp_exception *_exception)
{
    assert(_r);

    [[maybe_unused]] const auto _p = csp_unique_ptr_get(_r);

    if (!_r->_p)
    {
        const csp_shared_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

        return _this;
    }

    csp_cntrl_blk *const _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_cntrl));
    if (!_cntrl)
    {
        const csp_shared_ptr _this = { ._p = nullptr, ._cntrl = nullptr };

        *_exception = CSP_BAD_ALLOC;

        return _this;
    }

    csp_cntrl_blk_init(_cntrl, _r->_p, _r->_d);
    const csp_shared_ptr _this = { ._p = csp_unique_ptr_release(_r), ._cntrl = _cntrl };

    *_exception = CSP_SUCCESS;

    assert(csp_shared_ptr_get(&_this) == _p);
    assert(!csp_unique_ptr_get(_r));
    assert(csp_shared_ptr_use_count(&_this) == 1);

    return _this;
}

void csp_shared_ptr_destroy(csp_shared_ptr *const _this)
{
    assert(_this);

    if (_this->_cntrl)
    {
        csp_cntrl_blk_release_shared(_this->_cntrl);
    }
}

csp_shared_ptr *csp_shared_ptr_copy_s(csp_shared_ptr *const _this, const csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp = csp_shared_ptr_init_copy_s(_r);
    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);

    return _this;
}

csp_shared_ptr *csp_shared_ptr_move_s(csp_shared_ptr *const _this, csp_shared_ptr *const _r)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp = csp_shared_ptr_init_move_s(_r);
    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);

    return _this;
}

csp_shared_ptr *csp_shared_ptr_move_u(csp_shared_ptr *const _this, csp_unique_ptr *const _r)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp = csp_shared_ptr_init_move_u(_r);
    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);

    return _this;
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

bool csp_shared_ptr_bool(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_p != nullptr;
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

void csp_shared_ptr_reset(csp_shared_ptr *const _this)
{
    assert(_this);

    csp_shared_ptr _tmp = csp_shared_ptr_init();
    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_p(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p, csp_exception *const _exception)
{
    assert(_this);

    csp_shared_ptr _tmp = csp_shared_ptr_init_p(_p, _exception);
    if (*_exception != CSP_SUCCESS)
    {
        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_pd(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p, const csp_shared_ptr_D _d, csp_exception *const _exception)
{
    assert(_this);

    csp_shared_ptr _tmp = csp_shared_ptr_init_pd(_p, _d, _exception);
    if (*_exception != CSP_SUCCESS)
    {
        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_p_copy_s(csp_shared_ptr *const _this, const csp_shared_ptr *const _r, csp_shared_ptr_T *const _p, csp_exception *const _exception)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp = csp_shared_ptr_init_p_copy_s(_p, _exception);
    if (*_exception != CSP_SUCCESS)
    {
        return;
    }

    csp_shared_ptr_swap(&_tmp, _this);
    csp_shared_ptr_destroy(&_tmp);
}

void csp_shared_ptr_reset_p_move_s(csp_shared_ptr *const _this, csp_shared_ptr *const _r, csp_shared_ptr_T *const _p, csp_exception *const _exception)
{
    assert(_this);
    assert(_r);

    csp_shared_ptr _tmp = csp_shared_ptr_init_p_move_s(_p, _exception);
    if (*_exception != CSP_SUCCESS)
    {
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

csp_shared_ptr csp_make_shared_for_overwrite(const size_t _size, csp_exception *const _exception)
{
    const auto _ptr = (unsigned char *)malloc(_size + sizeof(csp_cntrl_blk));
    if (!_ptr)
    {
        const csp_shared_ptr _r = { ._p = nullptr, ._cntrl = nullptr };

        *_exception = CSP_BAD_ALLOC;

        return _r;
    }

    const auto _p = (csp_shared_ptr_T *)_ptr;
    const auto _cntrl = (csp_cntrl_blk *)(_ptr + _size);

    csp_cntrl_blk_init(_cntrl, _p, csp_default_delete);
    const csp_shared_ptr _r = { ._p = _p, ._cntrl = _cntrl };

    *_exception = CSP_SUCCESS;

    return _r;
}

void csp_swap_s(csp_shared_ptr *const _x, csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    csp_shared_ptr_swap(_x, _y);
}

bool csp_shared_ptr_equal_to(const csp_shared_ptr *const _x, const csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p == _y->_p;
}

bool csp_shared_ptr_not_equal_to(const csp_shared_ptr *const _x, const csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p != _y->_p;
}

bool csp_shared_ptr_less(const csp_shared_ptr *const _x, const csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p < _y->_p;
}

bool csp_shared_ptr_less_equal(const csp_shared_ptr *const _x, const csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p <= _y->_p;
}

bool csp_shared_ptr_greater(const csp_shared_ptr *const _x, const csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p > _y->_p;
}

bool csp_shared_ptr_greater_equal(const csp_shared_ptr *const _x, const csp_shared_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p >= _y->_p;
}

size_t csp_shared_ptr_hash(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_p ? (size_t)_this->_p : 0;
}
