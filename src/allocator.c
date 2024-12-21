#include "csp/allocator.h"

#include <stdlib.h>

const csp_allocator* const csp_default_allocator = &(csp_allocator) { .allocate = malloc, .deallocate = free };
