#ifndef CSP_CNTRL_BLK_H
#define CSP_CNTRL_BLK_H

#ifdef __STD_NO_ATOMICS__
#warning "shared pointers require stdatomic.h for synchronization"
#else
#include <stdatomic.h>
#endif

typedef void csp_cntrl_blk_T;

typedef void (*csp_cntrl_blk_D)(csp_cntrl_blk_T *);

typedef struct csp_cntrl_blk csp_cntrl_blk;

csp_cntrl_blk *csp_cntrl_blk_pd(csp_cntrl_blk *_this, csp_cntrl_blk_T *_p, csp_cntrl_blk_D _d);

csp_cntrl_blk_D *csp_cntrl_blk_get_deleter(csp_cntrl_blk *_this);

long csp_cntrl_blk_use_count(const csp_cntrl_blk *_this);

csp_cntrl_blk *csp_cntrl_blk_lock(csp_cntrl_blk *_this);

void csp_cntrl_blk_add_shared(csp_cntrl_blk *_this);

void csp_cntrl_blk_release_shared(csp_cntrl_blk *_this);

void csp_cntrl_blk_add_weak(csp_cntrl_blk *_this);

void csp_cntrl_blk_release_weak(csp_cntrl_blk *_this);

#ifdef __STD_NO_ATOMICS__
#define CSP_ATOMIC_LONG long
#else
#define CSP_ATOMIC_LONG atomic_long
#endif

struct csp_cntrl_blk
{
    CSP_ATOMIC_LONG _shared_owners;
    CSP_ATOMIC_LONG _weak_owners;
    csp_cntrl_blk_T *_p;
    csp_cntrl_blk_D _d;
};

#endif