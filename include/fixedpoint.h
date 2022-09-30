
#ifndef __FIXEDPOINT_H__
#define __FIXEDPOINT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_STDFIX 

#include <stdfix.h>

    typedef accum fixpt_s1615;
    typedef frac fixpt_s31;

    typedef unsigned accum fixpt_1616;
    typedef unsigned frac fixpt_s31;

#else

#include <stdint.h>

    typedef int32_t fixpt_s1615;
    typedef int32_t fixpt_s31;


    typedef uint32_t fixpt_1616;
    typedef uint32_t fixpt_32;

#endif

#ifdef __cplusplus
}
#endif

#endif /* __FIXEDPOINT_H__ */

