#ifndef CSP_MTX_POOL_H
#define CSP_MTX_POOL_H

#include <threads.h>

[[nodiscard]] mtx_t* csp_mtx_pool_get(const void* _p);

#endif
