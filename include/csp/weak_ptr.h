#ifndef CSP_WEAK_PTR_H
#define CSP_WEAK_PTR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct csp_shared_ptr csp_shared_ptr;

typedef void csp_weak_ptr_T;

typedef struct csp_weak_ptr csp_weak_ptr;

typedef csp_weak_ptr_T csp_weak_ptr_element_type;

void csp_weak_ptr_init(csp_weak_ptr *_this);

void csp_weak_ptr_init_w_copy(csp_weak_ptr *_this, const csp_weak_ptr *_r);

void csp_weak_ptr_init_w_move(csp_weak_ptr *_this, csp_weak_ptr *_r);

void csp_weak_ptr_init_w_p_copy(csp_weak_ptr *_this, const csp_weak_ptr *_r, csp_weak_ptr_T *_p);

void csp_weak_ptr_init_w_p_move(csp_weak_ptr *_this, csp_weak_ptr *_r, csp_weak_ptr_T *_p);

void csp_weak_ptr_init_s_copy(csp_weak_ptr *_this, const csp_shared_ptr *_r);

void csp_weak_ptr_init_s_p_copy(csp_weak_ptr *_this, const csp_shared_ptr *_r, csp_weak_ptr_T *_p);

void csp_weak_ptr_destroy(csp_weak_ptr *_this);

long csp_weak_ptr_use_count(const csp_weak_ptr *_this);

bool csp_weak_ptr_expired(const csp_weak_ptr *_this);

csp_shared_ptr csp_weak_ptr_lock(const csp_weak_ptr *_this);

bool csp_weak_ptr_owner_before_s(const csp_weak_ptr *_this, const csp_shared_ptr *_r);

bool csp_weak_ptr_owner_before_w(const csp_weak_ptr *_this, const csp_weak_ptr *_r);

bool csp_weak_ptr_owner_equals_s(const csp_weak_ptr *_this, const csp_shared_ptr *_r);

bool csp_weak_ptr_owner_equals_w(const csp_weak_ptr *_this, const csp_weak_ptr *_r);

size_t csp_weak_ptr_owner_hash_value(const csp_weak_ptr *_this);

void csp_weak_ptr_reset(csp_weak_ptr *_this);

void csp_weak_ptr_swap(csp_weak_ptr *_this, csp_weak_ptr *_r);

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_weak_ptr_cleanup __attribute__((cleanup(csp_weak_ptr_destroy))) csp_weak_ptr
#endif
#endif

typedef struct csp_cntrl_blk csp_cntrl_blk;

struct csp_weak_ptr
{
    csp_weak_ptr_T *_p;
    csp_cntrl_blk *_cntrl;
};

#endif
