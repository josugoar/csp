#ifndef CSP_WEAK_PTR_H
#define CSP_WEAK_PTR_H

#include <stddef.h>

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_weak_ptr_cleanup __attribute__((cleanup(csp_weak_ptr_destroy))) csp_weak_ptr
#endif
#endif

typedef struct csp_shared_ptr csp_shared_ptr;

typedef void csp_weak_ptr_T;

/// @brief Weak reference to an object managed by csp_shared_ptr.
typedef struct csp_weak_ptr csp_weak_ptr;

/// @brief csp_weak_ptr_T.
typedef csp_weak_ptr_T csp_weak_ptr_element_type;

/// @brief Creates a new csp_weak_ptr.
[[nodiscard]] csp_weak_ptr csp_weak_ptr_init(void);

/// @brief Creates a new csp_weak_ptr.
[[nodiscard]] csp_weak_ptr csp_weak_ptr_init_copy_w(const csp_weak_ptr* _r);

/// @brief Creates a new csp_weak_ptr.
[[nodiscard]] csp_weak_ptr csp_weak_ptr_init_copy_s(const csp_shared_ptr* _r);

/// @brief Creates a new csp_weak_ptr.
[[nodiscard]] csp_weak_ptr csp_weak_ptr_init_move_w(csp_weak_ptr* _r);

/// @brief Destroys a csp_weak_ptr.
void csp_weak_ptr_destroy(csp_weak_ptr* _this);

/// @brief Assigns the csp_weak_ptr.
csp_weak_ptr* csp_weak_ptr_copy_w(csp_weak_ptr* _this, const csp_weak_ptr* _r);

/// @brief Assigns the csp_weak_ptr.
csp_weak_ptr* csp_weak_ptr_copy_s(csp_weak_ptr* _this, const csp_shared_ptr* _r);

/// @brief Assigns the csp_weak_ptr.
csp_weak_ptr* csp_weak_ptr_move_w(csp_weak_ptr* _this, csp_weak_ptr* _r);

/// @brief Swaps the managed objects.
void csp_weak_ptr_swap(csp_weak_ptr* _this, csp_weak_ptr* _r);

/// @brief Releases the ownership of the managed object.
void csp_weak_ptr_reset(csp_weak_ptr* _this);

/// @brief Returns the number of csp_shared_ptr objects that manage the object.
[[nodiscard]] long csp_weak_ptr_use_count(const csp_weak_ptr* _this);

/// @brief Checks whether the referenced object was already deleted.
[[nodiscard]] bool csp_weak_ptr_expired(const csp_weak_ptr* _this);

/// @brief Creates a csp_shared_ptr that manages the referenced object.
[[nodiscard]] csp_shared_ptr csp_weak_ptr_lock(const csp_weak_ptr* _this);

/// @brief Provides owner-based ordering of weak pointers.
[[nodiscard]] bool csp_weak_ptr_owner_before_s(const csp_weak_ptr* _this, const csp_shared_ptr* _r);

/// @brief Provides owner-based ordering of weak pointers.
[[nodiscard]] bool csp_weak_ptr_owner_before_w(const csp_weak_ptr* _this, const csp_weak_ptr* _r);

/// @brief Owner-based hash support for csp_weak_ptr.
[[nodiscard]] size_t csp_weak_ptr_owner_hash(const csp_weak_ptr* _this);

/// @brief Provides owner-based ordering of weak pointers.
[[nodiscard]] bool csp_weak_ptr_owner_equal_s(const csp_weak_ptr* _this, const csp_shared_ptr* _r);

/// @brief Provides owner-based ordering of weak pointers.
[[nodiscard]] bool csp_weak_ptr_owner_equal_w(const csp_weak_ptr* _this, const csp_weak_ptr* _r);

typedef struct csp_cntrl_blk csp_cntrl_blk;

struct csp_weak_ptr
{
    csp_weak_ptr_T* _p;
    csp_cntrl_blk* _cntrl;
};

#endif
