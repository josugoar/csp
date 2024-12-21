#ifndef CSP_ALLOCATOR_H
#define CSP_ALLOCATOR_H

#include <stddef.h>

typedef void csp_allocator_T;

typedef struct csp_allocator csp_allocator;

typedef csp_allocator_T csp_allocator_value_type;

extern const csp_allocator* const csp_default_allocator;

struct csp_allocator
{
    csp_allocator_T* (*allocate)(size_t _n);
    void (*deallocate)(csp_allocator_T* _p);
};

#endif
