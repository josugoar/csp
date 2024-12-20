#include "csp/unique_ptr.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void csp_default_delete(csp_default_delete_T* const _ptr)
{
    free(_ptr);
}

csp_unique_ptr csp_unique_ptr_init(void)
{
    const csp_unique_ptr _this = { ._p = nullptr, ._d = csp_default_delete };

    assert(!csp_unique_ptr_get(&_this));
    assert(*csp_unique_ptr_get_deleter_const(&_this) == csp_default_delete);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_p(csp_unique_ptr_T* const _p)
{
    const csp_unique_ptr _this = { ._p = _p, ._d = csp_default_delete };

    assert(csp_unique_ptr_get(&_this) == _p);
    assert(*csp_unique_ptr_get_deleter_const(&_this) == csp_default_delete);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_d(const csp_unique_ptr_D _d)
{
    const csp_unique_ptr _this = { ._p = nullptr, ._d = _d };

    assert(!csp_unique_ptr_get(&_this));
    assert(*csp_unique_ptr_get_deleter_const(&_this) == _d);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_pd(csp_unique_ptr_T* const _p, const csp_unique_ptr_D _d)
{
    const csp_unique_ptr _this = { ._p = _p, ._d = _d };

    assert(csp_unique_ptr_get(&_this) == _p);
    assert(*csp_unique_ptr_get_deleter_const(&_this) == _d);

    return _this;
}

csp_unique_ptr csp_unique_ptr_init_move_u(csp_unique_ptr* const _u)
{
    assert(_u);

    [[maybe_unused]] const auto _p = csp_unique_ptr_get(_u);

    const csp_unique_ptr _this = { ._p = csp_unique_ptr_release(_u), ._d = _u->_d };

    assert(csp_unique_ptr_get(&_this) == _p);
    assert(!csp_unique_ptr_get(_u));
    assert(*csp_unique_ptr_get_deleter_const(&_this) == *csp_unique_ptr_get_deleter(_u));

    return _this;
}

void csp_unique_ptr_destroy(csp_unique_ptr* const _this)
{
    assert(_this);
    assert(*csp_unique_ptr_get_deleter(_this));

    csp_unique_ptr_reset(_this);
}

csp_unique_ptr* csp_unique_ptr_move_u(csp_unique_ptr* const _this, csp_unique_ptr* const _u)
{
    assert(_this);
    assert(_u);

    [[maybe_unused]] const auto _p = csp_unique_ptr_get(_u);

    csp_unique_ptr_reset_p(_this, csp_unique_ptr_release(_u));
    _this->_d = _u->_d;

    assert(csp_unique_ptr_get(_this) == _p);
    assert(!csp_unique_ptr_get(_u));
    assert(*csp_unique_ptr_get_deleter(_this) == *csp_unique_ptr_get_deleter(_u));

    return _this;
}

csp_unique_ptr_T* csp_unique_ptr_get(const csp_unique_ptr* const _this)
{
    assert(_this);

    return _this->_p;
}

csp_unique_ptr_D* csp_unique_ptr_get_deleter(csp_unique_ptr* const _this)
{
    assert(_this);

    return &_this->_d;
}

const csp_unique_ptr_D* csp_unique_ptr_get_deleter_const(const csp_unique_ptr* const _this)
{
    assert(_this);

    return &_this->_d;
}

csp_unique_ptr_T* csp_unique_ptr_release(csp_unique_ptr* const _this)
{
    assert(_this);

    const auto _p = _this->_p;

    _this->_p = nullptr;

    assert(!csp_unique_ptr_get(_this));

    return _p;
}

void csp_unique_ptr_reset(csp_unique_ptr* const _this)
{
    csp_unique_ptr_reset_p(_this, nullptr);
}

void csp_unique_ptr_reset_p(csp_unique_ptr* const _this, csp_unique_ptr_T* const _p)
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

void csp_unique_ptr_swap(csp_unique_ptr* const _this, csp_unique_ptr* const _u)
{
    assert(_this);
    assert(_u);

    const auto _p = _this->_p;
    _this->_p = _u->_p;
    _u->_p = _p;

    const auto _d = _this->_d;
    _this->_d = _u->_d;
    _u->_d = _d;
}

csp_unique_ptr csp_make_unique(const size_t _size, const csp_unique_ptr_T* const _p, csp_exception* const _e)
{
    assert(_p);
    assert(_e);

    return csp_make_unique_d(_size, _p, csp_default_delete, _e);
}

csp_unique_ptr csp_make_unique_d(const size_t _size, const csp_unique_ptr_T* const _p, const csp_unique_ptr_D _d, csp_exception* const _e)
{
    assert(_p);
    assert(_e);

    const auto _u = csp_make_unique_for_overwrite_d(_size, _d, _e);
    if (*_e != CSP_SUCCESS)
    {
        return _u;
    }

    memcpy(_u._p, _p, _size);

    return _u;
}

csp_unique_ptr csp_make_unique_for_overwrite(const size_t _size, csp_exception* const _e)
{
    assert(_e);

    return csp_make_unique_for_overwrite_d(_size, csp_default_delete, _e);
}

csp_unique_ptr csp_make_unique_for_overwrite_d(const size_t _size, const csp_unique_ptr_D _d, csp_exception* const _e)
{
    assert(_e);

    const auto _ptr = (unsigned char*)malloc(_size);
    if (!_ptr)
    {
        *_e = CSP_BAD_ALLOC;

        return (csp_unique_ptr) { ._p = nullptr, ._d = nullptr };
    }

    const auto _p = (csp_unique_ptr_T*)_ptr;

    const csp_unique_ptr _u = { ._p = _p, ._d = _d };

    *_e = CSP_SUCCESS;

    return _u;
}

size_t csp_unique_ptr_hash(const csp_unique_ptr* const _this)
{
    assert(_this);

    return _this->_p ? (size_t)_this->_p : 0;
}
