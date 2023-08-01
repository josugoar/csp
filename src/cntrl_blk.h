#ifndef CSP_CNTRL_BLK_H
#define CSP_CNTRL_BLK_H

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
};

#endif
