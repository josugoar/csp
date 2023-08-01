#ifndef CSP_SCOPED_PTR_H
#define CSP_SCOPED_PTR_H

#include <stddef.h>

typedef void csp_scoped_ptr_T;

typedef struct csp_scoped_ptr csp_scoped_ptr;

typedef csp_scoped_ptr_T csp_scoped_ptr_element_type;

void csp_scoped_ptr_init(csp_scoped_ptr *_this);

void csp_scoped_ptr_init_p(csp_scoped_ptr *_this, csp_scoped_ptr_T *_p);

void csp_scoped_ptr_destroy(csp_scoped_ptr *_this);

csp_scoped_ptr_T *csp_scoped_ptr_get(const csp_scoped_ptr *_this);

size_t csp_scoped_ptr_hash(const csp_scoped_ptr *_this);

void csp_scoped_ptr_reset(csp_scoped_ptr *_this);

void csp_scoped_ptr_reset_p(csp_scoped_ptr *_this, csp_scoped_ptr_T *_p);

void csp_scoped_ptr_swap(csp_scoped_ptr *_this, csp_scoped_ptr *_u);

csp_scoped_ptr csp_make_scoped_for_overwrite(size_t _size);

// TODO
// csp_scoped_ptr csp_allocate_for_overwrite(const csp_allocator *_a, size_t _size);

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_scoped_ptr_cleanup __attribute__((cleanup(csp_scoped_ptr_destroy))) csp_scoped_ptr
#endif
#endif

struct csp_scoped_ptr
{
    csp_scoped_ptr_T *_p;
};

#endif
