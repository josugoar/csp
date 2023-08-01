#include "csp/scoped_ptr.h"

#include <assert.h>
#include <stdlib.h>

void csp_scoped_ptr_init(csp_scoped_ptr *const _this)
{
    assert(_this);

    _this->_p = NULL;

    assert(!csp_scoped_ptr_get(_this));
}

void csp_scoped_ptr_init_p(csp_scoped_ptr *const _this, csp_scoped_ptr_T *const _p)
{
    assert(_this);

    _this->_p = _p;

    assert(csp_scoped_ptr_get(_this) == _p);
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

size_t csp_scoped_ptr_hash(const csp_scoped_ptr *const _this)
{
    assert(_this);

    return _this->_p ? (size_t)_this->_p : 0;
}

void csp_scoped_ptr_reset(csp_scoped_ptr *const _this)
{
    csp_scoped_ptr_reset_p(_this, NULL);
}

void csp_scoped_ptr_reset_p(csp_scoped_ptr *const _this, csp_scoped_ptr_T *const _p)
{
    assert(_this);

    csp_scoped_ptr_T *const _old_p = _this->_p;

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

    csp_scoped_ptr_T *const _p = _this->_p;
    _this->_p = _b->_p;
    _b->_p = _p;
}

csp_scoped_ptr csp_make_scoped_for_overwrite(const size_t _size)
{
    csp_scoped_ptr _u;

    unsigned char *const _ptr = (unsigned char *)malloc(_size);
    if (!_ptr)
    {
        // TODO: error handling
        _u._p = NULL;

        return _u;
    }

    csp_scoped_ptr_T *const _p = (csp_scoped_ptr_T *)_ptr;

    _u._p = _p;

    return _u;
}

// TODO
// csp_scoped_ptr csp_allocate_for_overwrite(const csp_allocator *const _a, const size_t _size);

