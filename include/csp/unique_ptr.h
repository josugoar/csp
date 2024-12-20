#ifndef CSP_UNIQUE_PTR_H
#define CSP_UNIQUE_PTR_H

#include <stddef.h>

#include "csp/exception.h"

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_unique_ptr_cleanup __attribute__((cleanup(csp_unique_ptr_destroy))) csp_unique_ptr
#endif
#endif

typedef void csp_default_delete_T;

///@brief Default deleter for csp_unique_ptr.
void csp_default_delete(csp_default_delete_T* _ptr);

typedef void csp_unique_ptr_T;

typedef void (*csp_unique_ptr_D)(csp_unique_ptr_T*);

/// @brief Smart pointer with unique object ownership semantics.
typedef struct csp_unique_ptr csp_unique_ptr;

/// @brief csp_unique_ptr_T*.
typedef csp_unique_ptr_T* csp_unique_ptr_pointer;

/// @brief csp_unique_ptr_T, the type of the object managed by this csp_unique_ptr.
typedef csp_unique_ptr_T csp_unique_ptr_element_type;

/// @brief csp_unique_ptr_D, the function object, to be called from the destructor.
typedef csp_unique_ptr_D csp_unique_ptr_deleter_type;

/// @brief Creates a new csp_unique_ptr.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init(void);

/// @brief Creates a new csp_unique_ptr.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_p(csp_unique_ptr_T* _p);

/// @brief Creates a new csp_unique_ptr.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_d(csp_unique_ptr_D _d);

/// @brief Creates a new csp_unique_ptr.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_pd(csp_unique_ptr_T* _p, csp_unique_ptr_D _d);

/// @brief Creates a new csp_unique_ptr.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_move_u(csp_unique_ptr* _u);

/// @brief Destructs the managed object if such is present.
void csp_unique_ptr_destroy(csp_unique_ptr* _this);

/// @brief Assigns the csp_unique_ptr.
csp_unique_ptr* csp_unique_ptr_move_u(csp_unique_ptr* _this, csp_unique_ptr* _u);

/// @brief Returns a pointer to the managed object.
[[nodiscard]] csp_unique_ptr_T* csp_unique_ptr_get(const csp_unique_ptr* _this);

/// @brief Returns the deleter that is used for destruction of the managed object.
[[nodiscard]] csp_unique_ptr_D* csp_unique_ptr_get_deleter(csp_unique_ptr* _this);

/// @brief Returns the deleter that is used for destruction of the managed object.
[[nodiscard]] const csp_unique_ptr_D* csp_unique_ptr_get_deleter_const(const csp_unique_ptr* _this);

/// @brief Checks if there is an associated managed object.
[[nodiscard]] bool csp_unique_ptr_bool(const csp_unique_ptr* _this);

/// @brief Returns a pointer to the managed object and releases the ownership.
[[nodiscard]] csp_unique_ptr_T* csp_unique_ptr_release(csp_unique_ptr* _this);

/// @brief Replaces the managed object.
void csp_unique_ptr_reset(csp_unique_ptr* _this);

/// @brief Replaces the managed object.
void csp_unique_ptr_reset_p(csp_unique_ptr* _this, csp_unique_ptr_T* _p);

/// @brief Swaps the managed objects.
void csp_unique_ptr_swap(csp_unique_ptr* _this, csp_unique_ptr* _u);

/// @brief Creates a unique pointer that manages a new object.
[[nodiscard]] csp_unique_ptr csp_make_unique(size_t _size, const csp_unique_ptr_T* _p, csp_exception* _e);

/// @brief Creates a unique pointer that manages a new object.
[[nodiscard]] csp_unique_ptr csp_make_unique_d(size_t _size, const csp_unique_ptr_T* _p, csp_unique_ptr_D _d, csp_exception* _e);

/// @brief Creates a unique pointer that manages a new object.
[[nodiscard]] csp_unique_ptr csp_make_unique_for_overwrite(size_t _size, csp_exception* _e);

/// @brief Creates a unique pointer that manages a new object.
[[nodiscard]] csp_unique_ptr csp_make_unique_for_overwrite_d(size_t _size, csp_unique_ptr_D _d, csp_exception* _e);

/// @brief Compares to another csp_unique_ptr.
[[nodiscard]] bool csp_unique_ptr_equal_to(const csp_unique_ptr* _x, const csp_unique_ptr* _y);

/// @brief Compares to another csp_unique_ptr.
[[nodiscard]] bool csp_unique_ptr_not_equal_to(const csp_unique_ptr* _x, const csp_unique_ptr* _y);

/// @brief Compares to another csp_unique_ptr.
[[nodiscard]] bool csp_unique_ptr_less(const csp_unique_ptr* _x, const csp_unique_ptr* _y);

/// @brief Compares to another csp_unique_ptr.
[[nodiscard]] bool csp_unique_ptr_less_equal(const csp_unique_ptr* _x, const csp_unique_ptr* _y);

/// @brief Compares to another csp_unique_ptr.
[[nodiscard]] bool csp_unique_ptr_greater(const csp_unique_ptr* _x, const csp_unique_ptr* _y);

/// @brief Compares to another csp_unique_ptr.
[[nodiscard]] bool csp_unique_ptr_greater_equal(const csp_unique_ptr* _x, const csp_unique_ptr* _y);

/// @brief Hash support for csp_unique_ptr.
[[nodiscard]] size_t csp_unique_ptr_hash(const csp_unique_ptr* _this);

struct csp_unique_ptr
{
    csp_unique_ptr_T* _p;
    csp_unique_ptr_D _d;
};

#endif
