#ifndef CSP_UNIQUE_PTR_H
#define CSP_UNIQUE_PTR_H

#include <stddef.h>

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_unique_ptr_cleanup __attribute__((cleanup(csp_unique_ptr_destroy))) csp_unique_ptr
#endif
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

void csp_unique_ptr_init_u_move(csp_unique_ptr *_this, csp_unique_ptr *_u);

void csp_unique_ptr_destroy(csp_unique_ptr *_this);

csp_unique_ptr_T *csp_unique_ptr_get(const csp_unique_ptr *_this);

csp_unique_ptr_D *csp_unique_ptr_get_deleter(csp_unique_ptr *_this);

const csp_unique_ptr_D *csp_unique_ptr_get_deleter_const(const csp_unique_ptr *_this);

void csp_unique_ptr_hash(const csp_unique_ptr *_this);

csp_unique_ptr_T *csp_unique_ptr_release(csp_unique_ptr *_this);

void csp_unique_ptr_reset(csp_unique_ptr *_this);

void csp_unique_ptr_reset_p(csp_unique_ptr *_this, csp_unique_ptr_T *_p);

void csp_unique_ptr_swap(csp_unique_ptr *_this, csp_unique_ptr *_u);

csp_unique_ptr csp_make_unique_for_overwrite(size_t _size);

struct csp_unique_ptr
{
    csp_unique_ptr_T *_p;
    csp_unique_ptr_D _d;
};

#endif
