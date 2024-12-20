#include <assert.h>
#include <stdlib.h>

#include <csp.h>

typedef int A;

void test(void) {
  // Pointers of same type
  {
    A* const ptr1 = (A*)malloc(sizeof(*ptr1));
    const csp_unique_ptr p1 = csp_unique_ptr_init_p(ptr1);

    A* const ptr2 = (A*)malloc(sizeof(*ptr2));
    const csp_unique_ptr p2 = csp_unique_ptr_init_p(ptr2);

    assert(ptr1 != ptr2);
    assert(ptr1 != ptr2);
  }
  // Default-constructed pointers
  {
    const csp_unique_ptr p1 = csp_unique_ptr_init();
    A* const ptr1 = csp_unique_ptr_get(&p1);

    const csp_unique_ptr p2 = csp_unique_ptr_init();
    A* const ptr2 = csp_unique_ptr_get(&p2);

    assert(ptr1 == ptr2);
  }
}

int main(int, const char* [])
{
  test();

  return EXIT_SUCCESS;
}
