# csp

<p align="center">
  <img src="assets/csp.png" alt="csp" width="640" height="320" />
</p>

<h1></h1>

> ©️ 🧠 👉 C++ style smart pointers for C23

CSP (C Smart Pointers) is a C++ inspired smart pointer library for C23 (although it can be backported to C11 but wanted to check out the new features) specifically inspired by [Boost.SmartPtr](https://www.boost.org/doc/libs/1_82_0/libs/smart_ptr/doc/html/smart_ptr.html) and C++20 smart pointers from the [memory](https://en.cppreference.com/w/cpp/memory) header and the corresponding [libc++](https://github.com/llvm/llvm-project/tree/main/libcxx) implementation. It makes extensive use of modern C23 features such as `nullptr`, `constexpr`, attributes, unnamed arguments and `auto` type inference, empty initializers, as well as C11 atomics and threads, so a C23 compiler is required to build it. At the time of writing, only GCC is able to be used due to the lack of support of Clang, verify C23 support at [cppreference](https://en.cppreference.com/w/c/23).

## Features

* **Strict ownership semantics** up to what the C language allows. This is to say that ownership modifying operations must be explicitly stated in code and force the programmer to make the right decisions. Thanks to the "pass by value" nature of csp smart pointers, it is not possible (in a conventional fashion) to mix up types when passing pointers around.

* **RAII** (Resource Acquisition Is Initialization), thanks to the use of GCC's cleanup attribute. This is mainly for convenience and to avoid having to call each object's "destructor" (or equivalent in C) manually. Inspired by its use in the [systemd](https://news.ycombinator.com/item?id=11305142) codebase. Avoid unless specifically using [GNUC](https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html) *-std=gnu** instead of plain C. Also, having defined the API, it is possible to use a custom static analyzer to check wheater smart pointer destructors are called, which would "entirely" solve the memory allocation problem while defining strict ownership semantics.

* A **similar interface to C++** smart pointers, with the exception of having to aquire the pointer from the smart pointer using the `get` function to manipulate it. This is due to the lack of operator overloading in C. Aside from that, it follows the standard "object oriented" interface commonly used in C libraries that is similar to the `init` and `destroy` functions originally present in posix, the C standard library and others, except that constructors return a copy due to the inexpensive nature of smart pointers (this allows for a **very** succinct calling style).

* **Thread safe** reference counting, using C11 atomics and threads. Non local shared pointers make use of atomic operations to ensure thread safety and employ some of the optimizations present in libc++, altough not extensive.

* **Atomic** specializations of shared pointers. Shared pointers themselves are not atomic by default, but their reference counters are, which can cause data races if accessed concurrently. Atomic shared pointers are useful for when the shared pointer itself needs to be accessed concurrently, but they are implemented using mutexes, which are slower than intrinsic atomic operations, due to the (very heavily) increased complexity of pure atomics. They even have modern wait/notify support!

* **Generic** pointers thanks to the use of `void *`. This allows the use of any type of pointer for storage, **NOT** including function pointers (technically undefined bahaviour because of no guarantees of both sharing the same size). This approach, however, limits type safety, and while it is possible to use macro and `_Generic` magic to distpatch to the correct function depending on the number and type of the arguments at compile type, or generate the entire code from macros or use gnuc macro extensions, it gets messy extremely quickly and `void *` is usually the prefered solution, even by the standard.

* ~~Boost.SmartPtr **extended smart pointers** in the form of pointer types not present in the C++ standard library. They don't matter a whole lot in C++ because of weird class member packing optimizations with templates, but since C cannot do such things, they are useful to have as little (to practically none) overhead as possible.~~

  * This would require splitting the API into multiple similary flavoured pointer types (eg. local, nonlocal) and would kill generalizability.

* Implements common **optimizations** found in libc++ and Boost.SmartPtr, as well as the usual single allocations for the control block and the object itself using `csp_make_shared_for_overwrite` type functions.

* **Error handling** is achieved through out error pointers to preserve return values and allow for a very terse syntax which allows for straight forward ownership semantics. Similar to [GLib](https://docs.gtk.org/glib/error-reporting.html) error reporting approach without the *absolutely crazy* decision of aborting the program on insufficient memory.

## Possible improvements

* ~~Custom allocation support. This is very easy to implement but would require and additional overhead of at least one function pointer in each control block of shared pointers, since it is not possible to explicitelly do [EBO](https://en.cppreference.com/w/cpp/language/ebo) (Empty Base class Optimization) and it would require to add the corresponding allocation and deallocation functions inside it. CSP smart pointers already have an overhead of one function pointer when using the default deleter (free), which is minuscule in comparison to other "generic" solutions that always require memory allocation.~~

    * Since the overhead is negligible while the benefits are inmense, like being able to use smart pointers on some freestanding libc implementations that don't have malloc (even though now it seem to be required anyway), it is now implemented.

* ~~C++20 wait/notify atomic functions. They are not trivial to implement and are hard to test, so they are not a priority at the moment.~~

    * Wait/notify is supported BUT the implementation is does not use condition variables because of the overhead, instead, it uses a thread polling with timed backoff algorithm, so it's not atomic unfortunately.

* Lock-free atomic pointer specializations. Harder to implement. See [Inside STL: The atomic shared_ptr](https://devblogs.microsoft.com/oldnewthing/20241219-00/?p=110663).

## Example

```c
// [Optional] Ensure that the cleanup attribute is present
// as already explained
#if defined __has_attribute
#if __has_attribute(cleanup)
#define HAS_CLEANUP_ATTRIBUTE
#endif
#endif

#ifdef HAS_CLEANUP_ATTRIBUTE
#define csp_unique_ptr csp_unique_ptr_cleanup
#define csp_shared_ptr csp_shared_ptr_cleanup
#define csp_weak_ptr csp_weak_ptr_cleanup
#endif

// You can include the entire library with the csp header
#include <csp/csp.h>

// Or include only the specific smart pointer you want
#include <csp/unique_ptr>
#include <csp/shared_ptr>

// Smart pointers are passed by value (like normal pointers)
void unique_pointer_consumer(csp_unique_ptr);

void shared_pointer_consumer(csp_shared_ptr);

struct foo {
    int foo;
    int bar;
    int baz;
    int *allocated_memory;
};

void foo_destroy(struct foo *const p)
{
    free(p->allocated_memory);
    free(p);
}

// When construction the smart pointers notice how const
// correctness can still be respected without having to
// jump through hoops
int main(void)
{
    // CSP returns an error code if something goes wrong
    csp_exception e;

    // Create a unique pointer to a struct foo with a custom
    // deleter that is able to free the internal memory managed
    // by the pointer itself
    csp_unique_ptr u = csp_unique_ptr_init_pd(malloc(sizeof(struct foo)), foo_destroy, &e);

    // Recommended to check for errors
    if (e != CSP_SUCCESS) {
        // Handle error
    }

    // Initialize allocated memory to nothing to not crash the
    // program. Pointer can be initialized now or before making
    // it smart
    struct foo *foo = csp_unique_ptr_get(&u);
    foo->allocated_memory = nullptr;

    // Ownership is fully delegated to the consumer
    // when moving a unique pointer
    unique_pointer_consumer(csp_unique_ptr_init_move_u(&u));

    // Cleanup pointers don't need to be manually destroyed, but should
    // be avoided unless using gnuc extensions
    // Only one allocation is made (optimization)
    csp_shared_ptr r = csp_make_shared_for_overwrite(sizeof(int), &e);

    if (e != CSP_SUCCESS) {
        // Handle error
    }

    // Now it is even possible to initialize pointer directly, which is much more ergonomic
    int member = 10;
    auto other = csp_make_shared(sizeof(int), &member, &e);

    if (e != CSP_SUCCESS) {
        // Handle error
    }

    // "Constructors" type functions return a copy (inexpensive) of the
    // created shared pointer that stores a pointer to the internally
    // allocated memory

    // Access the pointer using the get function
    // You can manipulate the value this way
    int *const p = csp_shared_ptr_get(&r); // <-- CAN'T use type inference here

    // Access the deleter that will be used to destroy the object
    const auto d = csp_shared_ptr_get_deleter(&r);

    // Share ownership of the pointer with another function
    shared_pointer_consumer(csp_shared_ptr_init_copy_s(&r));

    // Let go of your ownership and pass it to the consumer
    shared_pointer_consumer(csp_shared_ptr_init_move_s(&r));

    // Remember, this check is not necessary, it is here for illustrative purposes only
#ifndef HAS_CLEANUP_ATTRIBUTE
    csp_shared_ptr_destroy(&other);
#endif

    // This will decrement the internal reference counter and destroy the
    // object if it reaches 0. A cleanup pointer does not need to call it.
#ifndef HAS_CLEANUP_ATTRIBUTE
    csp_shared_ptr_destroy(&r);
#endif

    // Now destroy will not do anything because u does not own the pointer
    // Again, recommended to destroy the pointer manually, there is no way
    // araound it in standard C without using extremelly costly abstractions
    // (glib) or macro magic, which is arguably worse than anything else
#ifndef HAS_CLEANUP_ATTRIBUTE
    csp_unique_ptr_destroy(&u);
#endif

    return 0;
}
```

## Building

```sh
# Clone the repository
git clone https://github.com/josugoar/csp.git

# Enter the repository
cd csp

# Build Doxygen documentation
doxygen Doxyfile

# Create a build directory
mkdir build

# Enter the build directory
cd build

# Configure the build
cmake ..

# Build the library
cmake --build .

# Run the tests
ctest

# Install the library
cmake --install .

# Or run CPack to generate the installers
# for .tar.gz, .sh or .tar.z generators
cpack -C CPackConfig.cmake
```

## Acknowledgements

* The brief documentation is "adapted" (copied) from [cppreference](https://en.cppreference.com/w/).

* Some of the reference counting code was inspired mostly by [libc++](https://github.com/llvm/llvm-project/tree/main/libcxx) and also [MSVC](https://github.com/microsoft/STL) and [libstdc++](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3). It was interesting seeing the different but similar approaches to the same problem.
