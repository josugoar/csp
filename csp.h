#ifndef CSP_H
#define CSP_H

#include <assert.h>
#include <stdlib.h>
#ifndef CSP_NO_SYNCHRONIZATION
#include <stdatomic.h>
#endif

typedef void csp_default_delete_T;

void csp_default_delete(csp_default_delete_T *_ptr);

typedef void csp_unique_ptr_T;

typedef void (*csp_unique_ptr_D)(csp_unique_ptr_T *);

typedef struct csp_unique_ptr csp_unique_ptr;

typedef csp_unique_ptr_T *csp_unique_ptr_pointer;

typedef csp_unique_ptr_T csp_unique_ptr_element_type;

typedef csp_unique_ptr_D csp_unique_ptr_deleter_type;

void csp_unique_ptr_init(csp_unique_ptr *_this);

void csp_unique_ptr_init_p(csp_unique_ptr *_this, csp_unique_ptr_T *_p);

void csp_unique_ptr_init_d(csp_unique_ptr *_this, csp_unique_ptr_D _d);

void csp_unique_ptr_init_pd(csp_unique_ptr *_this, csp_unique_ptr_T *_p, csp_unique_ptr_D _d);

void csp_unique_ptr_init_u(csp_unique_ptr *_this, csp_unique_ptr *_u);

void csp_unique_ptr_destroy(csp_unique_ptr *_this);

csp_unique_ptr_T *csp_unique_ptr_get(const csp_unique_ptr *_this);

csp_unique_ptr_D *csp_unique_ptr_get_deleter(csp_unique_ptr *_this);

const csp_unique_ptr_D *csp_unique_ptr_get_deleter_const(const csp_unique_ptr *_this);

csp_unique_ptr_T *csp_unique_ptr_release(csp_unique_ptr *_this);

void csp_unique_ptr_reset(csp_unique_ptr *_this);

void csp_unique_ptr_reset_p(csp_unique_ptr *_this, csp_unique_ptr_T *_p);

void csp_unique_ptr_swap(csp_unique_ptr *_this, csp_unique_ptr *_u);

csp_unique_ptr csp_make_unique_for_overwrite(size_t _size);

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_unique_ptr_cleanup __attribute__((cleanup(csp_unique_ptr_destroy))) csp_unique_ptr
#endif
#endif

typedef struct csp_weak_ptr csp_weak_ptr;

typedef void csp_shared_ptr_T;

typedef void (*csp_shared_ptr_D)(csp_shared_ptr_T *);

typedef struct csp_shared_ptr csp_shared_ptr;

typedef csp_shared_ptr_T csp_shared_ptr_element_type;

typedef csp_weak_ptr csp_shared_ptr_weak_type;

void csp_shared_ptr_init(csp_shared_ptr *_this);

void csp_shared_ptr_init_p(csp_shared_ptr *_this, csp_shared_ptr_T *_p);

void csp_shared_ptr_init_pd(csp_shared_ptr *_this, csp_shared_ptr_T *_p, csp_shared_ptr_D _d);

void csp_shared_ptr_init_s(csp_shared_ptr *_this, csp_shared_ptr *_s);

void csp_shared_ptr_init_s_const(csp_shared_ptr *_this, const csp_shared_ptr *_s);

void csp_shared_ptr_init_sp(csp_shared_ptr *_this, csp_shared_ptr *_s, csp_shared_ptr_T *_p);

void csp_shared_ptr_init_sp_const(csp_shared_ptr *_this, const csp_shared_ptr *_s, csp_shared_ptr_T *_p);

void csp_shared_ptr_init_w(csp_shared_ptr *_this, const csp_weak_ptr *_w);

void csp_shared_ptr_init_u(csp_shared_ptr *_this, csp_unique_ptr *_u);

void csp_shared_ptr_destroy(csp_shared_ptr *_this);

csp_shared_ptr_T *csp_shared_ptr_get(const csp_shared_ptr *_this);

csp_shared_ptr_D *csp_shared_ptr_get_deleter(const csp_shared_ptr *_this);

long csp_shared_ptr_use_count(const csp_shared_ptr *_this);

bool csp_shared_ptr_owner_before(const csp_shared_ptr *_this, const csp_shared_ptr *_s);

bool csp_shared_ptr_owner_before_w(const csp_shared_ptr *_this, const csp_weak_ptr *_w);

void csp_shared_ptr_reset(csp_shared_ptr *_this);

void csp_shared_ptr_reset_p(csp_shared_ptr *_this, csp_shared_ptr_T *_p);

void csp_shared_ptr_reset_pd(csp_shared_ptr *_this, csp_shared_ptr_T *_p, csp_shared_ptr_D _d);

void csp_shared_ptr_swap(csp_shared_ptr *_this, csp_shared_ptr *_s);

typedef void csp_weak_ptr_T;

typedef struct csp_weak_ptr csp_weak_ptr;

typedef csp_weak_ptr_T csp_weak_ptr_element_type;

void csp_weak_ptr_init(csp_weak_ptr *_this);

void csp_weak_ptr_init_w(csp_weak_ptr *_this, csp_weak_ptr *_w);

void csp_weak_ptr_init_w_const(csp_weak_ptr *_this, const csp_weak_ptr *_w);

void csp_weak_ptr_init_s(csp_weak_ptr *_this, const csp_shared_ptr *_s);

void csp_weak_ptr_destroy(csp_weak_ptr *_this);

long csp_weak_ptr_use_count(const csp_weak_ptr *_this);

bool csp_weak_ptr_owner_before(const csp_weak_ptr *_this, const csp_weak_ptr *_w);

bool csp_weak_ptr_owner_before_s(const csp_weak_ptr *_this, const csp_shared_ptr *_s);

bool csp_weak_ptr_expired(const csp_weak_ptr *_this);

csp_shared_ptr csp_weak_ptr_lock(const csp_weak_ptr *_this);

void csp_weak_ptr_reset(csp_weak_ptr *_this);

void csp_weak_ptr_swap(csp_weak_ptr *_this, csp_weak_ptr *_w);

csp_shared_ptr csp_make_shared_for_overwrite(size_t _size);

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_shared_ptr_cleanup __attribute__((cleanup(csp_shared_ptr_destroy))) csp_shared_ptr
#define csp_weak_ptr_cleanup __attribute__((cleanup(csp_weak_ptr_destroy))) csp_weak_ptr
#endif
#endif

void csp_default_delete(csp_default_delete_T *const _ptr)
{
    free(_ptr);
}

struct csp_unique_ptr
{
    csp_unique_ptr_T *_p;
    csp_unique_ptr_D _d;
};

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

void csp_unique_ptr_init_u(csp_unique_ptr *const _this, csp_unique_ptr *const _u)
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
        _u._p = NULL;
        _u._d = NULL;
    }
    else
    {
        _u._p = (csp_shared_ptr_T *)_ptr;
        _u._d = csp_default_delete;
    }

    return _u;
}

typedef void csp_cntrl_blk_T;

typedef void (*csp_cntrl_blk_D)(csp_cntrl_blk_T *);

typedef struct csp_cntrl_blk csp_cntrl_blk;

void csp_cntrl_blk_init(csp_cntrl_blk *_this, csp_cntrl_blk_T *_p, csp_cntrl_blk_D _d);

csp_cntrl_blk_D *csp_cntrl_blk_get_deleter(csp_cntrl_blk *_this);

long csp_cntrl_blk_use_count(const csp_cntrl_blk *_this);

csp_cntrl_blk *csp_cntrl_blk_lock(csp_cntrl_blk *_this);

void csp_cntrl_blk_add_shared(csp_cntrl_blk *_this);

void csp_cntrl_blk_release_shared(csp_cntrl_blk *_this);

void csp_cntrl_blk_add_weak(csp_cntrl_blk *_this);

void csp_cntrl_blk_release_weak(csp_cntrl_blk *_this);

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

struct csp_shared_ptr
{
    csp_shared_ptr_T *_p;
    csp_cntrl_blk *_cntrl;
};

void csp_shared_ptr_init(csp_shared_ptr *const _this)
{
    assert(_this);

    _this->_p = NULL;
    _this->_cntrl = NULL;

    assert(!csp_shared_ptr_get(_this));
    assert(csp_shared_ptr_use_count(_this) == 0);
}

void csp_shared_ptr_init_p(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p)
{
    assert(_this);

    csp_cntrl_blk *const _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_this->_cntrl));
    if (!_cntrl)
    {
        csp_default_delete(_p);

        _this->_p = NULL;
        _this->_cntrl = NULL;
    }
    else
    {
        _this->_p = _p;
        _this->_cntrl = _cntrl;

        csp_cntrl_blk_init(_this->_cntrl, _this->_p, csp_default_delete);

        assert(csp_shared_ptr_get(_this) == _p);
        assert(csp_shared_ptr_use_count(_this) == 1);
    }
}

void csp_shared_ptr_init_pd(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p, const csp_shared_ptr_D _d)
{
    assert(_this);

    csp_cntrl_blk *const _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_this->_cntrl));
    if (!_cntrl)
    {
        assert(_d);

        _d(_p);

        _this->_p = NULL;
        _this->_cntrl = NULL;
    }
    else
    {
        _this->_p = _p;
        _this->_cntrl = _cntrl;

        csp_cntrl_blk_init(_this->_cntrl, _this->_p, _d);

        assert(csp_shared_ptr_get(_this) == _p);
        assert(csp_shared_ptr_use_count(_this) == 1);
    }
}

void csp_shared_ptr_init_s(csp_shared_ptr *const _this, csp_shared_ptr *const _s)
{
    assert(_this);
    assert(_s);

    csp_shared_ptr_T *const _p = csp_shared_ptr_get(_s);

    _this->_p = _s->_p;
    _this->_cntrl = _s->_cntrl;

    _s->_p = NULL;
    _s->_cntrl = NULL;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(!csp_shared_ptr_get(_s));
}

void csp_shared_ptr_init_s_const(csp_shared_ptr *const _this, const csp_shared_ptr *const _s)
{
    assert(_this);
    assert(_s);

    _this->_p = _s->_p;
    _this->_cntrl = _s->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_shared(_this->_cntrl);
    }

    assert(csp_shared_ptr_get(_this) == csp_shared_ptr_get(_s));
    assert(csp_shared_ptr_use_count(_this) == csp_shared_ptr_use_count(_s));
}

void csp_shared_ptr_init_sp(csp_shared_ptr *const _this, csp_shared_ptr *const _s, csp_shared_ptr_T *const _p)
{
    assert(_this);
    assert(_s);

    _this->_p = _p;
    _this->_cntrl = _s->_cntrl;

    _s->_p = NULL;
    _s->_cntrl = NULL;

    assert(csp_shared_ptr_get(_this) == _p);
    assert(!csp_shared_ptr_get(_s));
}

void csp_shared_ptr_init_sp_const(csp_shared_ptr *const _this, const csp_shared_ptr *const _s, csp_shared_ptr_T *const _p)
{
    assert(_this);
    assert(_s);

    _this->_p = _p;
    _this->_cntrl = _s->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_shared(_this->_cntrl);
    }

    assert(csp_shared_ptr_get(_this) == _p);
}

void csp_shared_ptr_init_w(csp_shared_ptr *const _this, const csp_weak_ptr *const _w)
{
    assert(_this);
    assert(_w);

    csp_cntrl_blk *const _cntrl = _w->_cntrl ? csp_cntrl_blk_lock(_w->_cntrl) : _w->_cntrl;
    if (!_cntrl)
    {
        _this->_p = NULL;
        _this->_cntrl = NULL;
    }
    else
    {
        _this->_p = _w->_p;
        _this->_cntrl = _cntrl;
    }

    assert(csp_shared_ptr_use_count(_this) == csp_weak_ptr_use_count(_w));
}

void csp_shared_ptr_init_u(csp_shared_ptr *const _this, csp_unique_ptr *const _u)
{
    assert(_this);
    assert(_u);

    csp_shared_ptr_T *const _p = csp_unique_ptr_get(_u);

    if (!_p)
    {
        _this->_p = NULL;
        _this->_cntrl = NULL;
    }
    else
    {
        csp_cntrl_blk *const _cntrl = (csp_cntrl_blk *)malloc(sizeof(*_this->_cntrl));
        if (!_p)
        {
            _this->_p = NULL;
            _this->_cntrl = NULL;
        }
        else
        {
            _this->_p = _p;
            _this->_cntrl = _cntrl;

            csp_cntrl_blk_init(_this->_cntrl, _this->_p, *csp_unique_ptr_get_deleter(_u));

            csp_unique_ptr_release(_u);

            assert(csp_shared_ptr_get(_this) == _p);
            assert(csp_shared_ptr_use_count(_this) == 1);
        }
    }
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

    return _this->_cntrl ? csp_cntrl_blk_get_deleter(_this->_cntrl) : NULL;
}

long csp_shared_ptr_use_count(const csp_shared_ptr *const _this)
{
    assert(_this);

    return _this->_cntrl ? csp_cntrl_blk_use_count(_this->_cntrl) : 0;
}

bool csp_shared_ptr_owner_before(const csp_shared_ptr *const _this, const csp_shared_ptr *const _s)
{
    assert(_this);
    assert(_s);

    return _this->_cntrl < _s->_cntrl;
}

bool csp_shared_ptr_owner_before_w(const csp_shared_ptr *const _this, const csp_weak_ptr *const _w)
{
    assert(_this);
    assert(_w);

    return _this->_cntrl < _w->_cntrl;
}

void csp_shared_ptr_reset(csp_shared_ptr *const _this)
{
    assert(_this);

    csp_shared_ptr _s;
    csp_shared_ptr_init(&_s);

    csp_shared_ptr_swap(&_s, _this);

    csp_shared_ptr_destroy(&_s);
}

void csp_shared_ptr_reset_p(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p)
{
    assert(_this);

    csp_shared_ptr _s;

    csp_shared_ptr_init_p(&_s, _p);
    if (!csp_shared_ptr_get(&_s))
    {
        _this->_p = NULL;
        _this->_cntrl = NULL;
    }
    else
    {
        csp_shared_ptr_swap(&_s, _this);

        csp_shared_ptr_destroy(&_s);
    }
}

void csp_shared_ptr_reset_pd(csp_shared_ptr *const _this, csp_shared_ptr_T *const _p, const csp_shared_ptr_D _d)
{
    assert(_this);

    csp_shared_ptr _s;
    
    csp_shared_ptr_init_pd(&_s, _p, _d);
    if (!csp_shared_ptr_get(&_s))
    {
        _this->_p = NULL;
        _this->_cntrl = NULL;
    }
    else
    {
        csp_shared_ptr_swap(&_s, _this);

        csp_shared_ptr_destroy(&_s);
    }
}

void csp_shared_ptr_swap(csp_shared_ptr *const _this, csp_shared_ptr *const _s)
{
    assert(_this);
    assert(_s);

    csp_shared_ptr_T *const _p = _this->_p;
    _this->_p = _s->_p;
    _s->_p = _p;

    csp_cntrl_blk *const _cntrl = _this->_cntrl;
    _this->_cntrl = _s->_cntrl;
    _s->_cntrl = _cntrl;
}

struct csp_weak_ptr
{
    csp_weak_ptr_T *_p;
    csp_cntrl_blk *_cntrl;
};

void csp_weak_ptr_init(csp_weak_ptr *const _this)
{
    assert(_this);

    _this->_p = NULL;
    _this->_cntrl = NULL;

    assert(csp_weak_ptr_use_count(_this) == 0);
}

void csp_weak_ptr_init_w(csp_weak_ptr *const _this, csp_weak_ptr *const _w)
{
    assert(_this);
    assert(_w);

    _this->_p = _w->_p;
    _this->_cntrl = _w->_cntrl;

    _w->_p = NULL;
    _w->_cntrl = NULL;

    assert(csp_weak_ptr_use_count(_w) == 0);
}

void csp_weak_ptr_init_w_const(csp_weak_ptr *const _this, const csp_weak_ptr *const _w)
{
    assert(_this);
    assert(_w);

    _this->_p = _w->_p;
    _this->_cntrl = _w->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_weak(_this->_cntrl);
    }

    assert(csp_weak_ptr_use_count(_this) == csp_weak_ptr_use_count(_w));
}

void csp_weak_ptr_init_s(csp_weak_ptr *const _this, const csp_shared_ptr *const _s)
{
    assert(_this);
    assert(_s);

    _this->_p = _s->_p;
    _this->_cntrl = _s->_cntrl;

    if (_this->_cntrl)
    {
        csp_cntrl_blk_add_weak(_this->_cntrl);
    }

    assert(csp_weak_ptr_use_count(_this) == csp_shared_ptr_use_count(_s));
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

bool csp_weak_ptr_owner_before(const csp_weak_ptr *const _this, const csp_weak_ptr *const _w)
{
    assert(_this);
    assert(_w);

    return _this->_cntrl < _w->_cntrl;
}

bool csp_weak_ptr_owner_before(const csp_weak_ptr *const _this, const csp_shared_ptr *const _s)
{
    assert(_this);
    assert(_s);

    return _this->_cntrl < _s->_cntrl;
}

bool csp_weak_ptr_expired(const csp_weak_ptr *const _this)
{
    assert(_this);

    return csp_weak_ptr_use_count(_this) == 0;
}

csp_shared_ptr csp_weak_ptr_lock(const csp_weak_ptr *const _this)
{
    assert(_this);

    csp_shared_ptr _s;

    _s._cntrl = _this->_cntrl ? csp_cntrl_blk_lock(_this->_cntrl) : _this->_cntrl;
    if (_s._cntrl)
    {
        _s._p = _this->_p;
    }

    return _s;
}

void csp_weak_ptr_reset(csp_weak_ptr *const _this)
{
    assert(_this);

    csp_weak_ptr _w;
    csp_weak_ptr_init(&_w);

    csp_weak_ptr_swap(&_w, _this);

    csp_weak_ptr_destroy(&_w);
}

void csp_weak_ptr_swap(csp_weak_ptr *const _this, csp_weak_ptr *const _w)
{
    assert(_this);
    assert(_w);

    csp_weak_ptr_T *const _p = _this->_p;
    _this->_p = _w->_p;
    _w->_p = _p;

    csp_cntrl_blk *const _cntrl = _this->_cntrl;
    _this->_cntrl = _w->_cntrl;
    _w->_cntrl = _cntrl;
}

csp_shared_ptr csp_make_shared_for_overwrite(const size_t _size)
{
    csp_shared_ptr _s;

    unsigned char *const _ptr = (unsigned char *)malloc(_size + sizeof(_s._cntrl));
    if (!_ptr)
    {
        _s._p = NULL;
        _s._cntrl = NULL;
    }
    else
    {
        _s._p = (csp_shared_ptr_T *)_ptr;
        _s._cntrl = (csp_cntrl_blk *)(_ptr + _size);

        csp_cntrl_blk_init(_s._cntrl, _s._p, csp_default_delete);
    }

    return _s;
}

#endif
