
#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <printf.h>

#ifdef NDEBUG

#define assert(x)

#else

extern void _exit (int status);

#define __expr_to_string(expr) #expr
#define __expanded_expr_to_string(expr) __expr_to_string(expr)
#define assert(expr) do {                                              \
    if (!(expr)) {                                                     \
        printf ( __FILE__ ":" __expanded_expr_to_string(__LINE__)      \
                " Assertion \"" __expr_to_string(expr) "\" failed." ); \
        _exit(0x0badc0d3d);                                            \
    }                                                                  \
} while (0)

#endif

#endif /* __ASSERT_H__ */

