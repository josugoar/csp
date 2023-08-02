#include "csp/unique_ptr.h"

#include <assert.h>
#include <stdlib.h>

void csp_default_delete(csp_default_delete_T *const _ptr)
{
    free(_ptr);
}

csp_unique_ptr csp_unique_ptr_init(void)
{
    const csp_unique_ptr _this = { ._p = nullptr, ._d = csp_default_delete };

    assert(!csp_unique_ptr_get(&_this));
    assert(*csp_unique_ptr_get_deleter(&_this) == csp_default_delete);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_p(csp_unique_ptr_T *const _p)
{
    const csp_unique_ptr _this = { ._p = _p, ._d = csp_default_delete };

    assert(csp_unique_ptr_get(&_this) == _p);
    assert(*csp_unique_ptr_get_deleter(&_this) == csp_default_delete);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_d(const csp_unique_ptr_D _d)
{
    const csp_unique_ptr _this = { ._p = nullptr, ._d = _d };

    assert(!csp_unique_ptr_get(&_this));
    assert(*csp_unique_ptr_get_deleter(&_this) == _d);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_pd(csp_unique_ptr_T *const _p, const csp_unique_ptr_D _d)
{
    const csp_unique_ptr _this = { ._p = _p, ._d = _d };

    assert(csp_unique_ptr_get(&_this) == _p);
    assert(*csp_unique_ptr_get_deleter(&_this) == _d);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_move_u(csp_unique_ptr *const _r)
{
    assert(_r);

    [[maybe_unused]] const auto _p = csp_unique_ptr_get(_r);

    const csp_unique_ptr _this = { ._p = csp_unique_ptr_release(_r), ._d = _r->_d };

    assert(csp_unique_ptr_get(&_this) == _p);
    assert(!csp_unique_ptr_get(_r));
    assert(*csp_unique_ptr_get_deleter(&_this) == *csp_unique_ptr_get_deleter(_r));

    return _this;
}

void csp_unique_ptr_destroy(csp_unique_ptr *const _this)
{
    assert(_this);
    assert(*csp_unique_ptr_get_deleter(_this));

    if (_this->_p)
    {
        _this->_d(_this->_p);
    }
}

csp_unique_ptr *csp_unique_ptr_move_u(csp_unique_ptr *_this, csp_unique_ptr *_r)
{
    assert(_this);
    assert(_r);

    csp_unique_ptr_reset_p(_this, csp_unique_ptr_release(_r));

    _this->_d = _r->_d;

    assert(!csp_unique_ptr_get(_r));

    return _this;
}

csp_unique_ptr_T *csp_unique_ptr_get(const csp_unique_ptr *const _this)
{
    assert(_this);

    return _this->_p;
}

csp_unique_ptr_D *csp_unique_ptr_get_deleter(csp_unique_ptr *const _this)
{
    assert(_this);

    return &_this->_d;
}

const csp_unique_ptr_D *csp_unique_ptr_get_deleter_const(const csp_unique_ptr *const _this)
{
    assert(_this);

    return &_this->_d;
}

csp_unique_ptr_T *csp_unique_ptr_release(csp_unique_ptr *const _this)
{
    assert(_this);

    const auto _p = _this->_p;

    _this->_p = nullptr;

    assert(!csp_unique_ptr_get(_this));

    return _p;
}

void csp_unique_ptr_reset(csp_unique_ptr *const _this)
{
    csp_unique_ptr_reset_p(_this, nullptr);
}

void csp_unique_ptr_reset_p(csp_unique_ptr *const _this, csp_unique_ptr_T *const _p)
{
    assert(_this);
    assert(*csp_unique_ptr_get_deleter(_this));

    const auto _old_p = _this->_p;

    _this->_p = _p;

    assert(csp_unique_ptr_get(_this) == _p);

    if (_old_p)
    {
        _this->_d(_old_p);
    }
}

void csp_unique_ptr_swap(csp_unique_ptr *const _this, csp_unique_ptr *const _r)
{
    assert(_this);
    assert(_r);

    const auto _p = _this->_p;
    _this->_p = _r->_p;
    _r->_p = _p;

    const auto _d = _this->_d;
    _this->_d = _r->_d;
    _r->_d = _d;
}

csp_unique_ptr csp_make_unique_for_overwrite(const size_t _size, csp_exception *const _exception)
{
    csp_unique_ptr _r;

    const auto _ptr = (unsigned char *)malloc(_size);
    if (!_ptr)
    {
        _r._p = nullptr;
        _r._d = nullptr;

        *_exception = CSP_BAD_ALLOC;

        return _r;
    }

    const auto _p = (csp_unique_ptr_T *)_ptr;
    const auto _d = csp_default_delete;

    _r._p = _p;
    _r._d = _d;

    *_exception = CSP_SUCCESS;

    return _r;
}

void csp_swap_u(csp_unique_ptr *const _x, csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    csp_unique_ptr_swap(_x, _y);
}

bool csp_unique_ptr_equal_to(const csp_unique_ptr *const _x, const csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p == _y->_p;
}

bool csp_unique_ptr_less(const csp_unique_ptr *const _x, const csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p < _y->_p;
}

bool csp_unique_ptr_greater(const csp_unique_ptr *const _x, const csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p > _y->_p;
}

bool csp_unique_ptr_not_equal_to(const csp_unique_ptr *const _x, const csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return !(_x->_p == _y->_p);
}

bool csp_unique_ptr_less_equal(const csp_unique_ptr *const _x, const csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return !(_x->_p < _y->_p);
}

bool csp_unique_ptr_greater_equal(const csp_unique_ptr *const _x, const csp_unique_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return !(_x->_p > _y->_p);
}

size_t csp_unique_ptr_hash(const csp_unique_ptr *const _this)
{
    assert(_this);

    return _this->_p ? (size_t)_this->_p : 0;
}
