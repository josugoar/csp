#include <assert.h>
#include <stdlib.h>

#include <csp.h>

typedef int A;

void test(void) {
    A *const p1 = (A *)malloc(sizeof(*p1));
    *p1 = 1;
    csp_unique_ptr s1 = csp_unique_ptr_init_p(p1);

    A *const p2 = (A *)malloc(sizeof(*p2));
    *p2 = 2;
    csp_unique_ptr s2 = csp_unique_ptr_init_p(p2);

    assert(csp_unique_ptr_get(&s1) == p1);
    assert(csp_unique_ptr_get(&s2) == p2);

    csp_unique_ptr_swap(&s1, &s2);

    assert(csp_unique_ptr_get(&s1) == p2);
    assert(csp_unique_ptr_get(&s2) == p1);

    csp_unique_ptr_destroy(&s1);
    csp_unique_ptr_destroy(&s2);
}

int main(int, const char *[])
{
    test();

    return EXIT_SUCCESS;
}

