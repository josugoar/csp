# C Smart Pointers (CSP)

CSP is a C++ inspired smart pointer library for C23 specifically inspired by [Boost.SmartPtr](https://www.boost.org/doc/libs/1_82_0/libs/smart_ptr/doc/html/smart_ptr.html) and C++20 smart pointers from [memory](https://en.cppreference.com/w/cpp/memory) and [libc++](https://github.com/llvm/llvm-project/tree/main/libcxx) implementation. It makes extensive use of modern C23 features such as `nullptr`, `constexpr`, attributes and `auto` type inference, as well as C11 atomics and threads, so a C23 compiler is required to build it. As of August 1 2023 only GCC is able to be used due to the lack of constexpr support of Clang, check support in [cppreference](https://en.cppreference.com/w/c/23).

## Features

* **RAII** (Resource Acquisition Is Initialization), thanks to the use of GCC's cleanup attribute. This is mainly for convenience and to avoid having to call each objects "destructor" (or equivalent in C) manually. Inspired by its use in the [systemd](https://news.ycombinator.com/item?id=11305142) codebase. Avoid unless specifically using [GNUC](https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html) *-std=gnu** instead of plain C.

* A **similar interface to C++** smart pointers, with the exception of having to aquire the pointer from the smart pointer using the `get` function. This is due to the lack of operator overloading in C. Aside from that, it follows the standard "object oriented" interface commonly used in C libraries that is similar to the `init` and `destroy` functions originally present in posix, the C standard library and others.

* **Thread safe** reference counting, using C11 atomics and threads. Non local shared pointers make use of atomic operations to ensure thread safety and employ some of the optimizations present in libc++, altough not extensive.

* **Atomic** specializations of shared pointers. Shared pointers themselves are not atomic by default, but their reference counters are, which can cause data races if accessed concurrently. Atomic shared pointers are useful for when the shared pointer itself needs to be accessed concurrently, but they are implemented using mutexes, which are slower than intrinsic atomic operations, due to the (very heavily) increased complexity of pure atomics.

* **Generic** pointers thanks to the use of `void *`. This allows the use of any type of pointer for storage, **NOT** including function pointers (technically undefined bahaviour because of no guarantees of both sharing the same size). This approach, however, limits type safety, and while it is possible to use macro and `_Generic` magic to distpatch to the correct function depending on the number and type of the arguments at compile type, or generate the entire code from macros or use gnuc macro extensions, it gets messy extremely quickly and `void *` is usually the prefered solution, even by the standard.

* Boost.SmartPtr **extended smart pointers** in the form of pointer types not present in the C++ standard library. They don't matter a whole lot in C++ because of weird class member packing optimizations with templates, but since C cannot do such things, they are useful to have as little (to practically none) overhead as possible.

* Implements common **optimizations** found in libc++ and Boost.SmartPtr, as well as the usual single allocations for the control block and the object itself using `csp_make_shared_for_overwrite` type functions.

## Example

```c
// You can include the entire library with the csp header
#include <csp/csp.h>

// Or include only the specific smart pointer you want
#include <csp/shared_ptr>

int main(void)
{
    // cleanup pointers don't need to be manually destroyed, but should
    // be avoided unless using gnuc extensions
    const csp_shared_ptr_cleanup r = csp_make_shared_for_overwrite(sizeof(int));

    // Recommended to check for errors obviously

    // csp_make_shared_for_overwrite type functions return a copy 
    // (inexpensive) of the created shared pointer that stores a pointer to
    // the internally allocated memory

    // Access the pointer using the get function
    int *const p = csp_shared_ptr_get(&r);

    // Access the deleter that will be used to destroy the object
    const auto d = csp_shared_ptr_get_deleter(&r);

    // This will decrement the internal reference counter and destroy the
    // object if it reaches 0. A cleanup pointer does not need to call it
    csp_shared_ptr_destroy(&r);

    return 0;
}
```

## Building

```sh
# Clone the repository
git clone https://github.com/josugoar/csp.git

# Enter the repository
cd csp

# [Optional] Build Doxygen documentation
doxygen Doxyfile

# Create a build directory
mkdir build

# Enter the build directory
cd build

# Configure the build
cmake ..

# Build the library
cmake --build .

# Install the library
cmake --install .
```
