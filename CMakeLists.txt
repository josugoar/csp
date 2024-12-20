cmake_minimum_required(VERSION 3.21.0)
project(csp
        VERSION 3.0.0
        DESCRIPTION "C++ style smart pointers for C23"
        HOMEPAGE_URL "https://github.com/josugoar/csp"
        LANGUAGES C)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(csp SHARED
            "${PROJECT_SOURCE_DIR}/src/allocator.c"
            "${PROJECT_SOURCE_DIR}/src/atomic_shared_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/atomic_weak_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/cntrl_blk.c"
            "${PROJECT_SOURCE_DIR}/src/mtx_pool.c"
            "${PROJECT_SOURCE_DIR}/src/shared_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/unique_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/weak_ptr.c")

target_include_directories(csp
                           PUBLIC "${PROJECT_SOURCE_DIR}/include"
                           PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_sources(csp
               PUBLIC FILE_SET HEADERS
               BASE_DIRS "${PROJECT_SOURCE_DIR}/include"
               FILES "${PROJECT_SOURCE_DIR}/include/csp.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/allocator.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/atomic_shared_ptr.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/atomic_weak_ptr.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/exception.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/shared_ptr.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/unique_ptr.h"
                     "${PROJECT_SOURCE_DIR}/include/csp/weak_ptr.h")

install(TARGETS csp FILE_SET HEADERS)

include(CTest)

add_executable(unique_ptr_cmp "${PROJECT_SOURCE_DIR}/test/unique_ptr/cmp.c")
target_link_libraries(unique_ptr_cmp PRIVATE csp)
add_test(NAME unique_ptr_cmp
         COMMAND "unique_ptr_cmp")

add_executable(unique_ptr_swap "${PROJECT_SOURCE_DIR}/test/unique_ptr/swap.c")
target_link_libraries(unique_ptr_swap PRIVATE csp)
add_test(NAME unique_ptr_swap
         COMMAND "unique_ptr_swap")

include(InstallRequiredSystemLibraries)
set(CPACK_PROJECT_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "josugoar")
set(CPACK_PROJECT_VERSION "${PROJECT_VERSION}")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
include(CPack)
