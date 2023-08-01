#include "csp/unique_ptr.h"

#include <assert.h>
#include <stdlib.h>

void csp_default_delete(csp_default_delete_T *const _ptr)
{
    free(_ptr);
}

void csp_unique_ptr_init(csp_unique_ptr *const _this)
{
    assert(_this);

    _this->_p = NULL;
    _this->_d = csp_default_delete;

    assert(!csp_unique_ptr_get(_this));
    assert(*csp_unique_ptr_get_deleter(_this) == csp_default_delete);
}

void csp_unique_ptr_init_p(csp_unique_ptr *const _this, csp_unique_ptr_T *const _p)
{
    assert(_this);

    _this->_p = _p;
    _this->_d = csp_default_delete;

    assert(csp_unique_ptr_get(_this) == _p);
    assert(*csp_unique_ptr_get_deleter(_this) == csp_default_delete);
}

void csp_unique_ptr_init_d(csp_unique_ptr *const _this, const csp_unique_ptr_D _d)
{
    assert(_this);

    _this->_p = NULL;
    _this->_d = _d;

    assert(!csp_unique_ptr_get(_this));
    assert(*csp_unique_ptr_get_deleter(_this) == _d);
}

void csp_unique_ptr_init_pd(csp_unique_ptr *const _this, csp_unique_ptr_T *const _p, const csp_unique_ptr_D _d)
{
    assert(_this);

    _this->_p = _p;
    _this->_d = _d;

    assert(csp_unique_ptr_get(_this) == _p);
    assert(*csp_unique_ptr_get_deleter(_this) == _d);
}

void csp_unique_ptr_init_u_move(csp_unique_ptr *const _this, csp_unique_ptr *const _u)
{
    assert(_this);
    assert(_u);

    csp_unique_ptr_T *const _p = csp_unique_ptr_get(_u);

    _this->_p = _u->_p;
    _this->_d = _u->_d;

    _u->_p = NULL;

    assert(csp_unique_ptr_get(_this) == _p);
    assert(!csp_unique_ptr_get(_u));
    assert(*csp_unique_ptr_get_deleter(_this) == *csp_unique_ptr_get_deleter(_u));
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

    csp_unique_ptr_T *const _p = _this->_p;

    _this->_p = NULL;

    assert(!csp_unique_ptr_get(_this));

    return _p;
}

void csp_unique_ptr_reset(csp_unique_ptr *const _this)
{
    csp_unique_ptr_reset_p(_this, NULL);
}

void csp_unique_ptr_reset_p(csp_unique_ptr *const _this, csp_unique_ptr_T *const _p)
{
    assert(_this);
    assert(*csp_unique_ptr_get_deleter(_this));

    csp_unique_ptr_T *const _old_p = _this->_p;

    _this->_p = _p;

    assert(csp_unique_ptr_get(_this) == _p);

    if (_old_p)
    {
        _this->_d(_old_p);
    }
}

void csp_unique_ptr_swap(csp_unique_ptr *const _this, csp_unique_ptr *const _u)
{
    assert(_this);
    assert(_u);

    csp_unique_ptr_T *const _p = _this->_p;
    _this->_p = _u->_p;
    _u->_p = _p;

    const csp_unique_ptr_D _d = _this->_d;
    _this->_d = _u->_d;
    _u->_d = _d;
}

csp_unique_ptr csp_make_unique_for_overwrite(const size_t _size)
{
    csp_unique_ptr _u;

    unsigned char *const _ptr = (unsigned char *)malloc(_size);
    if (!_ptr)
    {
        // TODO: error handling
        _u._p = NULL;
        _u._d = NULL;

        return _u;
    }

    csp_unique_ptr_T *const _p = (csp_unique_ptr_T *)_ptr;
    const csp_unique_ptr_D _d = csp_default_delete;

    _u._p = _p;
    _u._d = _d;

    return _u;
}

// TODO
// csp_unique_ptr csp_allocate_unique_for_overwrite(const csp_allocator *const _a,  const size_t _size);
