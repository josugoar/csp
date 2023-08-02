#include "csp/scoped_ptr.h"

#include <assert.h>

csp_scoped_ptr csp_scoped_ptr_init(void)
{
    const csp_scoped_ptr _this = { ._p = nullptr };

    assert(!csp_scoped_ptr_get(&_this));

    return _this;
}

csp_scoped_ptr csp_scoped_ptr_init_p(csp_scoped_ptr_T *const _p)
{
    const csp_scoped_ptr _this = { ._p = _p };

    assert(csp_scoped_ptr_get(&_this) == _p);

    return _this;
}

void csp_scoped_ptr_destroy(csp_scoped_ptr *const _this)
{
    assert(_this);

    if (_this->_p)
    {
        free(_this->_p);
    }
}

csp_scoped_ptr_T *csp_scoped_ptr_get(const csp_scoped_ptr *const _this)
{
    assert(_this);

    return _this->_p;
}

[[nodiscard]] const bool csp_scoped_ptr_bool(const csp_scoped_ptr *const _this)
{
    assert(_this);

    return _this->_p != nullptr;
}

void csp_scoped_ptr_reset(csp_scoped_ptr *const _this)
{
    csp_scoped_ptr_reset_p(_this, nullptr);
}

void csp_scoped_ptr_reset_p(csp_scoped_ptr *const _this, csp_scoped_ptr_T *const _p)
{
    assert(_this);

    const auto _old_p = _this->_p;

    _this->_p = _p;

    assert(csp_scoped_ptr_get(_this) == _p);

    if (_old_p)
    {
        free(_old_p);
    }
}

void csp_scoped_ptr_swap(csp_scoped_ptr *const _this, csp_scoped_ptr *const _b)
{
    assert(_this);
    assert(_b);

    const auto _p = _this->_p;
    _this->_p = _b->_p;
    _b->_p = _p;
}

void csp_swap_s(csp_scoped_ptr *const _x, csp_scoped_ptr *const _y)
{
    assert(_x);
    assert(_y);

    csp_scoped_ptr_swap(_x, _y);
}

[[nodiscard]] bool csp_scoped_ptr_equal_to(const csp_scoped_ptr *const _x, const csp_scoped_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p == _y->_p;
}

[[nodiscard]] bool csp_scoped_ptr_not_equal_to(const csp_scoped_ptr *const _x, const csp_scoped_ptr *const _y)
{
    assert(_x);
    assert(_y);

    return _x->_p != _y->_p;
}

