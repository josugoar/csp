#ifndef CSP_UNIQUE_PTR_H
#define CSP_UNIQUE_PTR_H

#include <stddef.h>

#include <csp/exception.h>

#if defined __has_attribute
#if __has_attribute(cleanup)
#define csp_unique_ptr_cleanup __attribute__((cleanup(csp_unique_ptr_destroy))) csp_unique_ptr
#endif
#endif

typedef void csp_default_delete_T;

/// @brief Calls free on _ptr.
void csp_default_delete(csp_default_delete_T *_ptr);

typedef void csp_unique_ptr_T;

typedef void (*csp_unique_ptr_D)(csp_unique_ptr_T *);

/// A unique pointer is an object that owns another object and manages that
/// other object through a pointer. More precisely, a unique pointer is an
/// object u that stores a pointer to a second object p and will dispose of p
/// when u is itself destroyed. In this context, u is said to own p.
///
/// The mechanism by which u disposes of p is known as p’s associated deleter, a
/// function object whose correct invocation results in p’s appropriate
/// disposition (typically its deletion).
///
/// Let the notation u.p denote the pointer stored by u, and let u.d denote the
/// associated deleter. Upon request, u can reset (replace) u.p and u.d with
/// another pointer and deleter, but properly disposes of its owned object via
/// the associated deleter before such replacement is considered completed.
typedef struct csp_unique_ptr csp_unique_ptr;

typedef csp_unique_ptr_T *csp_unique_ptr_pointer;

typedef csp_unique_ptr_T csp_unique_ptr_element_type;

typedef csp_unique_ptr_D csp_unique_ptr_deleter_type;

/// @brief Creates a csp_unique_ptr object that owns nothing, initializing the
///        stored pointer with nullptr and the stored deleter with csp_default_delete.
/// @post  csp_unique_ptr_get(_this) == nullptr. csp_unique_ptr_get_deleter(_this)
///        returns a pointer to the stored deleter.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init(void);

/// @brief Creates a csp_unique_ptr which owns _p, initializing the stored
///        pointer with _p and initializing the stored deleter with csp_default_delete.
/// @post  csp_unique_ptr_get(_this) == _p. csp_unique_ptr_get_deleter(_this)
///        returns a pointer to the stored deleter.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_p(csp_unique_ptr_T *_p);

/// @brief Creates a csp_unique_ptr object that owns nothing, initializing the
///        stored pointer with nullptr and the stored deleter with _d.
/// @post  csp_unique_ptr_get(_this) == nullptr. csp_unique_ptr_get_deleter(_this)
///        returns a pointer to the stored deleter.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_d(csp_unique_ptr_D _d);

/// @brief Creates a csp_unique_ptr which owns _p, initializing the stored
///        pointer with _p and initializing the stored deleter with _d.
/// @post  csp_unique_ptr_get(_this) == _p. csp_unique_ptr_get_deleter(_this)
///        returns a pointer to the stored deleter.
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_pd(csp_unique_ptr_T *_p, csp_unique_ptr_D _d);

/// @brief Creates a csp_unique_ptr from _r. The deleter is initialized with
///        _r's deleter.
/// @pre   _r must not be null.
/// @post  csp_unique_ptr_get(_this) yields the value csp_unique_ptr_get(_r)
///        yielded before the creation. csp_unique_ptr_get(_r) == nullptr.
///        csp_unique_ptr_get_deleter(_this) returns a pointer to the stored
///        deleter that was initialized with csp_unique_ptr_get_deleter(_r).
[[nodiscard]] csp_unique_ptr csp_unique_ptr_init_move_u(csp_unique_ptr *_r);

/// @brief If csp_unique_ptr_get(_this) == nullptr there are no effects.
///        Otherwise (*csp_unique_ptr_get_deleter(_this))(csp_unique_ptr_get(_this)).
/// @pre   The expression (*csp_unique_ptr_get_deleter(_this))(csp_unique_ptr_get(_this))
///        shall have well-defined behaviour. _this must not be null.
void csp_unique_ptr_destroy(csp_unique_ptr *_this);

/// @brief Calls csp_unique_ptr_reset(_this, csp_unique_ptr_release(_r))
///        followed by *csp_unique_ptr_get_deleter(_this) = *csp_unique_ptr_get_deleter(_r).
/// @pre   _this must not be null. _r must not be null.
/// @post  csp_unique_ptr_get(_r) == nullptr.
csp_unique_ptr *csp_unique_ptr_move_u(csp_unique_ptr *_this, csp_unique_ptr *_r);

/// @pre     _this must not be null.
/// @returns The stored pointer.
[[nodiscard]] csp_unique_ptr_T *csp_unique_ptr_get(const csp_unique_ptr *_this);

/// @pre     _this must not be null.
/// @returns A pointer to the stored deleter.
[[nodiscard]] csp_unique_ptr_D *csp_unique_ptr_get_deleter(csp_unique_ptr *_this);

/// @pre     _this must not be null.
/// @returns A pointer to the stored deleter.
[[nodiscard]] const csp_unique_ptr_D *csp_unique_ptr_get_deleter_const(const csp_unique_ptr *_this);

/// @pre     _this must not be null.
/// @post    csp_unique_ptr_get(_this) == nullptr.
/// @returns The value csp_unique_ptr_get(_this) had at the start of the call to
///          csp_unique_ptr_release
csp_unique_ptr_T *csp_unique_ptr_release(csp_unique_ptr *_this);

/// @brief Assigns nullptr to the stored pointer, and then and only if the old
///        value of the stored pointer, _old_ptr, was not equal to nullptr,
///        calls (*csp_unique_ptr_get_deleter(_this))(_old_ptr).
/// @note  The order of the operations is significant because the call to
///        csp_unique_ptr_get_deleter(_this) may destroy _this.
/// @pre   The expression (*csp_unique_ptr_get_deleter(_this))(csp_unique_ptr_get(_this))
///        shall have well-defined behaviour. _this must not be null.
/// @post  csp_unique_ptr_get(_this) == nullptr.
/// @note  The postcondition does not hold if the call to csp_unique_ptr_get_deleter(_this)
///        destroys _this since csp_unique_ptr_get(_this) is no longer a valid
///        expression.
void csp_unique_ptr_reset(csp_unique_ptr *_this);

/// @brief Assigns _p to the stored pointer, and then and only if the old
///        value of the stored pointer, _old_ptr, was not equal to nullptr,
///        calls (*csp_unique_ptr_get_deleter(_this))(_old_ptr).
/// @note  The order of the operations is significant because the call to
///        csp_unique_ptr_get_deleter(_this) may destroy _this.
/// @pre   The expression (*csp_unique_ptr_get_deleter(_this))(csp_unique_ptr_get(_this))
///        shall have well-defined behaviour. _this must not be null.
/// @post  csp_unique_ptr_get(_this) == _p.
/// @note  The postcondition does not hold if the call to csp_unique_ptr_get_deleter(_this)
///        destroys _this since csp_unique_ptr_get(_this) is no longer a valid
///        expression.
void csp_unique_ptr_reset_p(csp_unique_ptr *_this, csp_unique_ptr_T *_p);

/// @brief Swap the stored pointers and the stored deleters of _this and _r.
/// @pre   _this must not be null. _r must not be null.
void csp_unique_ptr_swap(csp_unique_ptr *_this, csp_unique_ptr *_r);

/// @returns csp_unique_ptr_init_p(_this, malloc(_size)).
[[nodiscard]] csp_unique_ptr csp_make_unique_for_overwrite(size_t _size, csp_exception *_exception);

/// @brief Calls csp_unique_ptr_swap(_x, _y).
/// @pre   _x must not be null. _y must not be null.
void csp_swap_u(csp_unique_ptr *_x, csp_unique_ptr *_y);

/// @pre     _x must not be null. _y must not be null.
/// @returns csp_unique_ptr_get(_x) == csp_unique_ptr_get(_y).
[[nodiscard]] bool csp_unique_ptr_equal_to(const csp_unique_ptr *_x, const csp_unique_ptr *_y);

/// @pre     _x must not be null. _y must not be null.
/// @returns csp_unique_ptr_get(_x) < csp_unique_ptr_get(_y).
[[nodiscard]] bool csp_unique_ptr_less(const csp_unique_ptr *_x, const csp_unique_ptr *_y);

/// @pre     _x must not be null. _y must not be null.
/// @returns csp_unique_ptr_get(_x) > csp_unique_ptr_get(_y).
[[nodiscard]] bool csp_unique_ptr_greater(const csp_unique_ptr *_x, const csp_unique_ptr *_y);

/// @pre     _x must not be null. _y must not be null.
/// @returns !(csp_unique_ptr_get(_x) == csp_unique_ptr_get(_y)).
[[nodiscard]] bool csp_unique_ptr_not_equal_to(const csp_unique_ptr *_x, const csp_unique_ptr *_y);

/// @pre     _x must not be null. _y must not be null.
/// @returns !(csp_unique_ptr_get(_x) < csp_unique_ptr_get(_y)).
[[nodiscard]] bool csp_unique_ptr_less_equal(const csp_unique_ptr *_x, const csp_unique_ptr *_y);

/// @pre     _x must not be null. _y must not be null.
/// @returns !(csp_unique_ptr_get(_x) > csp_unique_ptr_get(_y)).
[[nodiscard]] bool csp_unique_ptr_greater_equal(const csp_unique_ptr *_x, const csp_unique_ptr *_y);

/// @pre     _this must not be null.
/// @returns Hash of the stored pointer of _this.
[[nodiscard]] size_t csp_unique_ptr_hash(const csp_unique_ptr *_this);

struct csp_unique_ptr
{
    csp_unique_ptr_T *_p;
    csp_unique_ptr_D _d;
};

#endif
