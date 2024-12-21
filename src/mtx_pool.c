#include "mtx_pool.h"

constexpr auto csp_mtx_count = 32;
static auto csp_mtx_pool = (mtx_t[csp_mtx_count]){};

static auto csp_mtx_pool_flag = (once_flag)ONCE_FLAG_INIT;

static void csp_mtx_pool_init(void);

mtx_t* csp_mtx_pool_get(const void* const _p)
{
    call_once(&csp_mtx_pool_flag, csp_mtx_pool_init);

    return &csp_mtx_pool[(_p ? (size_t)_p : 0) & (csp_mtx_count - 1)];
}

void csp_mtx_pool_init(void)
{
    for (auto _i = 0; _i < csp_mtx_count; ++_i)
    {
        mtx_init(&csp_mtx_pool[_i], mtx_plain);
    }
}
