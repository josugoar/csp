#include "mtx_pool.h"

constexpr size_t csp_mtx_count = 32;
static mtx_t csp_mtx_pool[csp_mtx_count];

static csp_exception *const csp_mtx_pool_e;

void csp_mtx_pool_init(void)
{
    for (size_t _i = 0; _i < csp_mtx_count; ++_i)
    {
        if (mtx_init(&csp_mtx_pool[_i], mtx_plain) != thrd_success)
        {
            *csp_mtx_pool_e = CSP_BAD_ATOMIC;

            return;
        }
    }

    *csp_mtx_pool_e = CSP_SUCCESS;
}

mtx_t *csp_mtx_pool_get(const void *const _p, csp_exception *const _e)
{
    if (*csp_mtx_pool_e != CSP_SUCCESS)
    {
        *_e = *csp_mtx_pool_e;

        return nullptr;
    }

    *_e = CSP_SUCCESS;

    return &csp_mtx_pool[(_p ? (size_t)_p : 0) & (csp_mtx_count - 1)];
}
