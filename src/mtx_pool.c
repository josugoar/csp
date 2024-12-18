#include "mtx_pool.h"

constexpr size_t csp_mtx_count = 32;
static mtx_t csp_mtx_pool[csp_mtx_count];

static once_flag csp_mtx_pool_flag = ONCE_FLAG_INIT;

static csp_exception csp_mtx_pool_e = CSP_BAD_ATOMIC;

static void csp_mtx_pool_init(void);

mtx_t* csp_mtx_pool_get(const void* const _p, csp_exception* const _e)
{
    call_once(&csp_mtx_pool_flag, csp_mtx_pool_init);
    if (csp_mtx_pool_e != CSP_SUCCESS)
    {
        *_e = csp_mtx_pool_e;

        return nullptr;
    }

    return &csp_mtx_pool[(_p ? (size_t)_p : 0) & (csp_mtx_count - 1)];
}

void csp_mtx_pool_init(void)
{
    for (size_t _i = 0; _i < csp_mtx_count; ++_i)
    {
        if (mtx_init(&csp_mtx_pool[_i], mtx_plain) != thrd_success)
        {
            return;
        }
    }

    csp_mtx_pool_e = CSP_SUCCESS;
}
