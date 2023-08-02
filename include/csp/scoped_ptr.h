#ifndef CSP_SCOPED_PTR_H
#define CSP_SCOPED_PTR_H

#include <stddef.h>

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_scoped_ptr_cleanup __attribute__((cleanup(csp_scoped_ptr_destroy))) csp_scoped_ptr
#endif
#endif

typedef void csp_scoped_ptr_T;

typedef struct csp_scoped_ptr csp_scoped_ptr;

/// @brief csp_scoped_ptr_T, the type of the object managed by this csp_scoped_ptr.
typedef csp_scoped_ptr_T csp_scoped_ptr_element_type;

/// @brief Creates a new csp_scoped_ptr.
[[nodiscard]] csp_scoped_ptr csp_scoped_ptr_init(void);

/// @brief Creates a new csp_scoped_ptr.
[[nodiscard]] csp_scoped_ptr csp_scoped_ptr_init_p(csp_scoped_ptr_T *_p);

/// @brief Destructs the managed object if such is present.
void csp_scoped_ptr_destroy(csp_scoped_ptr *_this);

/// @brief Returns a pointer to the managed object.
[[nodiscard]] csp_scoped_ptr_T *csp_scoped_ptr_get(const csp_scoped_ptr *_this);

/// @brief Checks if there is an associated managed object.
[[nodiscard]] const bool csp_scoped_ptr_bool(const csp_scoped_ptr *_this);

/// @brief Replaces the managed object.
void csp_scoped_ptr_reset(csp_scoped_ptr *_this);

/// @brief Replaces the managed object.
void csp_scoped_ptr_reset_p(csp_scoped_ptr *_this, csp_scoped_ptr_T *_p);

/// @brief Swaps the managed objects.
void csp_scoped_ptr_swap(csp_scoped_ptr *_this, csp_scoped_ptr *_r);

/// @brief Swaps the managed objects.
void csp_swap_s(csp_scoped_ptr *_x, csp_scoped_ptr *_y);

/// @brief Compares to another csp_scoped_ptr.
[[nodiscard]] bool csp_scoped_ptr_equal_to(const csp_scoped_ptr *_x, const csp_scoped_ptr *_y);

/// @brief Compares to another csp_scoped_ptr.
[[nodiscard]] bool csp_scoped_ptr_not_equal_to(const csp_scoped_ptr *_x, const csp_scoped_ptr *_y);

struct csp_scoped_ptr
{
    csp_scoped_ptr_T *_p;
};

#endif
