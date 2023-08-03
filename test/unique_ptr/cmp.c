#include <assert.h>
#include <stdlib.h>

#include <csp.h>

typedef int A;

void test(void) {
  // Pointers of same type
  {
    A *const ptr1 = (A *)malloc(sizeof(*ptr1));
    const csp_unique_ptr p1 = csp_unique_ptr_init_p(ptr1);

    A *const ptr2 = (A *)malloc(sizeof(*ptr2));
    const csp_unique_ptr p2 = csp_unique_ptr_init_p(ptr2);

    assert(!csp_unique_ptr_equal_to(&p1, &p2));
    assert(csp_unique_ptr_not_equal_to(&p1, &p2));
    assert(csp_unique_ptr_less(&p1, &p2) == (ptr1 < ptr2));
    assert(csp_unique_ptr_less_equal(&p1, &p2) == (ptr1 <= ptr2));
    assert(csp_unique_ptr_greater(&p1, &p2)== (ptr1 > ptr2));
    assert(csp_unique_ptr_greater_equal(&p1, &p2) == (ptr1 >= ptr2));
  }
  // Default-constructed pointers
  {
    const csp_unique_ptr p1 = csp_unique_ptr_init();
    const csp_unique_ptr p2 = csp_unique_ptr_init();
    assert(csp_unique_ptr_equal_to(&p1, &p2));
  }
}

int main(int, const char *[])
{
    test();

    return EXIT_SUCCESS;
}
