cmake_minimum_required(VERSION 3.21.0)
project(csp
        VERSION 1.0.0
        DESCRIPTION "C++ style smart pointers for C"
        HOMEPAGE_URL "https://github.com/josugoar/csp"
        LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(csp SHARED
            "${PROJECT_SOURCE_DIR}/src/atomic_shared_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/atomic_weak_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/cntrl_blk.c"
            "${PROJECT_SOURCE_DIR}/src/cntrl_blk.h"
            "${PROJECT_SOURCE_DIR}/src/local_shared_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/scoped_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/shared_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/unique_ptr.c"
            "${PROJECT_SOURCE_DIR}/src/weak_ptr.c")

target_include_directories(csp PUBLIC
                           "${PROJECT_SOURCE_DIR}/include")

include(InstallRequiredSystemLibraries)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR josugoar)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
include(CPack)
