#ifndef CSP_MTX_POOL_H
#define CSP_MTX_POOL_H

#include <threads.h>

#include "csp/exception.h"

void csp_mtx_pool_init(void);

mtx_t *csp_mtx_pool_get(const void *_p, csp_exception *_e);

#endif
