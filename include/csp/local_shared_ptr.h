#ifndef CSP_LOCAL_SHARED_PTR_H
#define CSP_LOCAL_SHARED_PTR_H

#include <stddef.h>

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_local_shared_ptr_cleanup __attribute__((cleanup(csp_local_shared_ptr_destroy))) csp_local_shared_ptr
#endif
#endif

typedef struct csp_shared_ptr csp_shared_ptr;

typedef struct csp_unique_ptr csp_unique_ptr;

typedef void csp_local_shared_ptr_T;

typedef void (*csp_local_shared_ptr_D)(csp_local_shared_ptr_T *);

typedef struct csp_local_shared_ptr csp_local_shared_ptr;

typedef csp_local_shared_ptr_T csp_local_shared_ptr_element_type;

void csp_local_shared_ptr_init(csp_local_shared_ptr *_this);

void csp_local_shared_ptr_init_p(csp_local_shared_ptr *_this, csp_local_shared_ptr_T *_p);

void csp_local_shared_ptr_init_pd(csp_local_shared_ptr *_this, csp_local_shared_ptr_T *_p, csp_local_shared_ptr_D _d);

void csp_local_shared_ptr_init_ls_copy(csp_local_shared_ptr *_this, const csp_local_shared_ptr *_r);

void csp_local_shared_ptr_init_ls_move(csp_local_shared_ptr *_this, csp_local_shared_ptr *_r);

void csp_local_shared_ptr_init_ls_p_copy(csp_local_shared_ptr *_this, const csp_local_shared_ptr *_r, csp_local_shared_ptr_T *_p);

void csp_local_shared_ptr_init_ls_p_move(csp_local_shared_ptr *_this, csp_local_shared_ptr *_r, csp_local_shared_ptr_T *_p);

void csp_local_shared_ptr_init_s_copy(csp_local_shared_ptr *_this, const csp_shared_ptr *_r);

void csp_local_shared_ptr_init_s_move(csp_local_shared_ptr *_this, csp_shared_ptr *_r);

void csp_local_shared_ptr_init_u_move(csp_local_shared_ptr *_this, csp_unique_ptr *_r);

void csp_local_shared_ptr_destroy(csp_local_shared_ptr *_this);

csp_local_shared_ptr_T *csp_local_shared_ptr_get(const csp_local_shared_ptr *_this);

csp_local_shared_ptr_D *csp_local_shared_ptr_get_deleter(const csp_local_shared_ptr *_this);

long csp_local_shared_ptr_use_count(const csp_local_shared_ptr *_this);

bool csp_local_shared_ptr_owner_before_ls(const csp_local_shared_ptr *_this, const csp_local_shared_ptr *_r);

bool csp_local_shared_ptr_owner_equals_ls(const csp_local_shared_ptr *_this, const csp_local_shared_ptr *_r);

size_t csp_local_shared_ptr_owner_hash_value(const csp_local_shared_ptr *_this);

size_t csp_local_shared_ptr_hash(const csp_local_shared_ptr *_this);

void csp_local_shared_ptr_reset(csp_local_shared_ptr *_this);

void csp_local_shared_ptr_reset_p(csp_local_shared_ptr *_this, csp_local_shared_ptr_T *_p);

void csp_local_shared_ptr_reset_pd(csp_local_shared_ptr *_this, csp_local_shared_ptr_T *_p, csp_local_shared_ptr_D _d);

void csp_local_shared_ptr_reset_ls_p_copy(csp_local_shared_ptr *_this, const csp_local_shared_ptr *_r, csp_local_shared_ptr_T *_p);

void csp_local_shared_ptr_reset_ls_p_move(csp_local_shared_ptr *_this, csp_local_shared_ptr *_r, csp_local_shared_ptr_T *_p);

void csp_local_shared_ptr_swap(csp_local_shared_ptr *_this, csp_local_shared_ptr *_r);

csp_local_shared_ptr csp_make_local_shared_for_overwrite(size_t _size);

typedef struct csp_cntrl_blk csp_cntrl_blk;

struct csp_local_shared_ptr
{
    csp_local_shared_ptr_T *_p;
    csp_cntrl_blk *_cntrl;
};

#endif
