#ifndef CSP_H
#define CSP_H

#ifdef __STDC_NO_ATOMICS__
#warning "atomic pointers require stdatomic.h"
#else
#include <csp/atomic_shared_ptr.h>
#include <csp/atomic_weak_ptr.h>
#endif
#include <csp/shared_ptr.h>
#include <csp/unique_ptr.h>
#include <csp/weak_ptr.h>

#endif
