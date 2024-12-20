#ifndef CSP_SHARED_PTR_H
#define CSP_SHARED_PTR_H

#include <stddef.h>

#include "csp/exception.h"

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_shared_ptr_cleanup __attribute__((cleanup(csp_shared_ptr_destroy))) csp_shared_ptr
#endif
#endif

typedef struct csp_weak_ptr csp_weak_ptr;

typedef struct csp_unique_ptr csp_unique_ptr;

typedef void csp_shared_ptr_T;

typedef void (*csp_shared_ptr_D)(csp_shared_ptr_T*);

/// @brief Smart pointer with shared object ownership semantics.
typedef struct csp_shared_ptr csp_shared_ptr;

/// @brief csp_shared_ptr_T.
typedef csp_shared_ptr_T csp_shared_ptr_element_type;

/// @brief csp_weak_ptr.
typedef csp_weak_ptr csp_shared_ptr_weak_type;

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init(void);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_p(csp_shared_ptr_T* _p, csp_exception* _e);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_pd(csp_shared_ptr_T* _p, csp_shared_ptr_D _d, csp_exception* _e);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_p_copy_s(const csp_shared_ptr* _r, csp_shared_ptr_T* _p);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_p_move_s(csp_shared_ptr* _r, csp_shared_ptr_T* _p);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_copy_s(const csp_shared_ptr* _r);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_move_s(csp_shared_ptr* _r);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_copy_w(const csp_weak_ptr* _r, csp_exception* _e);

/// @brief Constructs new csp_shared_ptr.
[[nodiscard]] csp_shared_ptr csp_shared_ptr_init_move_u(csp_unique_ptr* _r, csp_exception* _e);

/// @brief Destructs the owned object if no more csp_shared_ptr link to it.
void csp_shared_ptr_destroy(csp_shared_ptr* _this);

/// @brief Assigns the csp_shared_ptr.
csp_shared_ptr* csp_shared_ptr_copy_s(csp_shared_ptr* _this, const csp_shared_ptr* _r);

/// @brief Assigns the csp_shared_ptr.
csp_shared_ptr* csp_shared_ptr_move_s(csp_shared_ptr* _this, csp_shared_ptr* _r);

/// @brief Assigns the csp_shared_ptr.
csp_shared_ptr* csp_shared_ptr_move_u(csp_shared_ptr* _this, csp_unique_ptr* _r, csp_exception* _e);

/// @brief Swaps the managed objects.
void csp_shared_ptr_swap(csp_shared_ptr* _this, csp_shared_ptr* _r);

/// @brief Replaces the managed object.
void csp_shared_ptr_reset(csp_shared_ptr* _this);

/// @brief Replaces the managed object.
void csp_shared_ptr_reset_p(csp_shared_ptr* _this, csp_shared_ptr_T* _p, csp_exception* _e);

/// @brief Replaces the managed object.
void csp_shared_ptr_reset_pd(csp_shared_ptr* _this, csp_shared_ptr_T* _p, csp_shared_ptr_D _d, csp_exception* _e);

/// @brief Returns the stored pointer.
[[nodiscard]] csp_shared_ptr_T* csp_shared_ptr_get(const csp_shared_ptr* _this);

/// @brief Returns the deleter, if owned.
[[nodiscard]] csp_shared_ptr_D* csp_shared_ptr_get_deleter(const csp_shared_ptr* _this);

/// @brief Returns the number of csp_shared_ptr objects referring to the same managed object.
[[nodiscard]] long csp_shared_ptr_use_count(const csp_shared_ptr* _this);

/// @brief Checks if the stored pointer is not null.
[[nodiscard]] bool csp_shared_ptr_bool(const csp_shared_ptr* _this);

/// @brief Provides owner-based ordering of shared pointers.
[[nodiscard]] bool csp_shared_ptr_owner_before_s(const csp_shared_ptr* _this, const csp_shared_ptr* _r);

/// @brief Provides owner-based ordering of shared pointers.
[[nodiscard]] bool csp_shared_ptr_owner_before_w(const csp_shared_ptr* _this, const csp_weak_ptr* _r);

/// @brief Owner-based hash support for csp_shared_ptr.
[[nodiscard]] size_t csp_shared_ptr_owner_hash(const csp_shared_ptr* _this);

/// @brief Provides owner-based ordering of shared pointers.
[[nodiscard]] bool csp_shared_ptr_owner_equal_s(const csp_shared_ptr* _this, const csp_shared_ptr* _r);

/// @brief Provides owner-based ordering of shared pointers.
[[nodiscard]] bool csp_shared_ptr_owner_equal_w(const csp_shared_ptr* _this, const csp_weak_ptr* _r);

/// @brief Creates a shared pointer that manages a new object.
[[nodiscard]] csp_shared_ptr csp_make_shared(size_t _size, const csp_shared_ptr_T* _p, csp_exception* _e);

/// @brief Creates a shared pointer that manages a new object.
[[nodiscard]] csp_shared_ptr csp_make_shared_d(size_t _size, const csp_shared_ptr_T* _p, csp_shared_ptr_D _d, csp_exception* _e);

/// @brief Creates a shared pointer that manages a new object.
[[nodiscard]] csp_shared_ptr csp_make_shared_for_overwrite(size_t _size, csp_exception* _e);

/// @brief Creates a shared pointer that manages a new object.
[[nodiscard]] csp_shared_ptr csp_make_shared_for_overwrite_d(size_t _size, csp_shared_ptr_D _d, csp_exception* _e);

/// @brief Compares to another csp_shared_ptr.
[[nodiscard]] bool csp_shared_ptr_equal_to(const csp_shared_ptr* _x, const csp_shared_ptr* _y);

/// @brief Compares to another csp_shared_ptr.
[[nodiscard]] bool csp_shared_ptr_not_equal_to(const csp_shared_ptr* _x, const csp_shared_ptr* _y);

/// @brief Compares to another csp_shared_ptr.
[[nodiscard]] bool csp_shared_ptr_less(const csp_shared_ptr* _x, const csp_shared_ptr* _y);

/// @brief Compares to another csp_shared_ptr.
[[nodiscard]] bool csp_shared_ptr_less_equal(const csp_shared_ptr* _x, const csp_shared_ptr* _y);

/// @brief Compares to another csp_shared_ptr.
[[nodiscard]] bool csp_shared_ptr_greater(const csp_shared_ptr* _x, const csp_shared_ptr* _y);

/// @brief Compares to another csp_shared_ptr.
[[nodiscard]] bool csp_shared_ptr_greater_equal(const csp_shared_ptr* _x, const csp_shared_ptr* _y);

/// @brief Hash support for csp_shared_ptr.
[[nodiscard]] size_t csp_shared_ptr_hash(const csp_shared_ptr* _this);

typedef struct csp_cntrl_blk csp_cntrl_blk;

struct csp_shared_ptr
{
    csp_shared_ptr_T* _p;
    csp_cntrl_blk* _cntrl;
};

#endif
