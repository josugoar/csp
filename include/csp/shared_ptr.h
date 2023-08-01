#ifndef CSP_SHARED_PTR_H
#define CSP_SHARED_PTR_H

#include <stddef.h>

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_shared_ptr_cleanup __attribute__((cleanup(csp_shared_ptr_destroy))) csp_shared_ptr
#endif
#endif

typedef struct csp_weak_ptr csp_weak_ptr;

typedef struct csp_unique_ptr csp_unique_ptr;

typedef void csp_shared_ptr_T;

typedef void (*csp_shared_ptr_D)(csp_shared_ptr_T *);

typedef struct csp_shared_ptr csp_shared_ptr;

typedef csp_shared_ptr_T csp_shared_ptr_element_type;

typedef csp_weak_ptr csp_shared_ptr_weak_type;

void csp_shared_ptr_init(csp_shared_ptr *_this);

void csp_shared_ptr_init_p(csp_shared_ptr *_this, csp_shared_ptr_T *_p);

void csp_shared_ptr_init_pd(csp_shared_ptr *_this, csp_shared_ptr_T *_p, csp_shared_ptr_D _d);

void csp_shared_ptr_init_s_copy(csp_shared_ptr *_this, const csp_shared_ptr *_r);

void csp_shared_ptr_init_s_move(csp_shared_ptr *_this, csp_shared_ptr *_r);

void csp_shared_ptr_init_s_p_copy(csp_shared_ptr *_this, const csp_shared_ptr *_r, csp_shared_ptr_T *_p);

void csp_shared_ptr_init_s_p_move(csp_shared_ptr *_this, csp_shared_ptr *_r, csp_shared_ptr_T *_p);

void csp_shared_ptr_init_w_copy(csp_shared_ptr *_this, const csp_weak_ptr *_r);

void csp_shared_ptr_init_u_move(csp_shared_ptr *_this, csp_unique_ptr *_r);

void csp_shared_ptr_destroy(csp_shared_ptr *_this);

csp_shared_ptr_T *csp_shared_ptr_get(const csp_shared_ptr *_this);

csp_shared_ptr_D *csp_shared_ptr_get_deleter(const csp_shared_ptr *_this);

long csp_shared_ptr_use_count(const csp_shared_ptr *_this);

bool csp_shared_ptr_owner_before_s(const csp_shared_ptr *_this, const csp_shared_ptr *_r);

bool csp_shared_ptr_owner_before_w(const csp_shared_ptr *_this, const csp_weak_ptr *_r);

bool csp_shared_ptr_owner_equals_s(const csp_shared_ptr *_this, const csp_shared_ptr *_r);

bool csp_shared_ptr_owner_equals_w(const csp_shared_ptr *_this, const csp_weak_ptr *_r);

size_t csp_shared_ptr_owner_hash_value(const csp_shared_ptr *_this);

size_t csp_shared_ptr_hash(const csp_shared_ptr *_this);

void csp_shared_ptr_reset(csp_shared_ptr *_this);

void csp_shared_ptr_reset_p(csp_shared_ptr *_this, csp_shared_ptr_T *_p);

void csp_shared_ptr_reset_pd(csp_shared_ptr *_this, csp_shared_ptr_T *_p, csp_shared_ptr_D _d);

void csp_shared_ptr_reset_s_p_copy(csp_shared_ptr *_this, const csp_shared_ptr *_r, csp_shared_ptr_T *_p);

void csp_shared_ptr_reset_s_p_move(csp_shared_ptr *_this, csp_shared_ptr *_r, csp_shared_ptr_T *_p);

void csp_shared_ptr_swap(csp_shared_ptr *_this, csp_shared_ptr *_r);

csp_shared_ptr csp_make_shared_for_overwrite(size_t _size);

typedef struct csp_cntrl_blk csp_cntrl_blk;

struct csp_shared_ptr
{
    csp_shared_ptr_T *_p;
    csp_cntrl_blk *_cntrl;
};

#endif
