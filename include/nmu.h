
#ifndef __NMU_H__
#define __NMU_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <constants.h>
#include <stdint.h>
#include <stdbool.h>
#include "fixedpoint.h"
#include "qpe-types.h"
#include "attributes.h"
#include "assert.h"

#ifndef DOXYGEN
#define TO_BOOL(x) ((x) ? 1 : 0)
#endif

//! @ingroup qpe
//! @defgroup nmu_wrap Neuromorphic Math Unit (NMU)
//! @brief Wrapper Function of Neuromorphic Math Unit (NMU).
//!
//! Includes:
//!  - **Hardware Exponential**
//!  - **Random Number Generator**
//! @{

typedef union {
    float f;
    int32_t w;
} float_word_t;

//! @brief Convert floating point value to the fixpoint (s1615) value
__static_inline int32_t float2fxpt (float arg)
{
#if 1
    /* faster and simpler one: */
    return (int32_t)(arg*32768.0);
#else
    float_word_t a;
    a.f = arg;
    int32_t exp = ((a.w>>23)&0xff)-127;
    int32_t s = 23-15-exp;
    int32_t fxpt = (0x00800000|(a.w&0x007fffff)) >> s;
    if (a.w& 0x80000000) {
        fxpt = -fxpt;
    }
    return fxpt;
#endif
}

//! @brief Convert fixpoint (s1615) value to floatingpoint value
__static_inline float fxpt2float (int arg)
{
#if 1
    /* faster and simpler one: */
    return ((float)arg)/32768.0;
#else 
    float_word_t res;
    res.w = arg;
    int32_t lz = __CLZ (res.w);
    if (unlikely (lz < 8)) {
        /* discard bits */
        res.w >>= (8-lz);
        res.w &= 0x007fffff;
        res.w |= (127+16-lz)<<23;
        return res.f;
    } else {
        /* padding */
        res.w <<= (lz-8);
        res.w &= 0x007fffff;
        res.w |= (127+16-lz)<<23;
        return res.f;
    }
#endif
}

//! @name Exponential acceleration unit
//! @{
//! @brief Pipelined access to the hardware exponential function accelerator.
//! The pipeline deep is 4.
//!

//! @brief Push one argument to the exp acceleration.
//! @see Globalvariable NMU_EXP
__static_inline void nmu_exp_calc (fixpt_s1615 x)
{
	*NMU_EXP = x;
}

//! @brief Push one argument to the exp accelerator.
//! @see Globalvariable NMU_EXP_FRAC
__static_inline void nmu_exp_frac_calc (fixpt_s31 x)
{
        *NMU_EXP_FRAC = x;
}

//! @brief Push one argument to the log accelerator.
//! @see Globalvariable NMU_LOG
__static_inline void nmu_log_calc (fixpt_s1615 x)
{
	*NMU_LOG = x;
}

//! @bried Push one argument to the log accelerator.
//! @see Globalvariable NMU_LOG_FRAC
__static_inline void nmu_log_frac_calc (fixpt_s31 x)
{
        *NMU_LOG_FRAC = x;
}

//! @brief Fetch argument of the exp acceleration.
//! @see Globalvariable NMU_EXP
__static_inline fixpt_s1615 nmu_exp_fetch (void)
{
    return *NMU_EXP;
}

//! @brief Fetch argument of the exp accelerator.
//! @see Globalvariable NMU_EXP_FRAC
__static_inline fixpt_s31 nmu_exp_frac_fetch (void)
{
    return *NMU_EXP_FRAC;
}

//! @brief Fetch argument of the exp accelerator.
//! @see Globalvariable NMU_LOG
__static_inline fixpt_s1615 nmu_log_fetch (void)
{
    return *NMU_LOG;
}

//! @brief Fetch argument of the log accelerator.
//! @see Globalvariable NMU_LOG_FRAC
__static_inline fixpt_s31 nmu_log_frac_fetch (void)
{
    return *NMU_LOG_FRAC;
}

//! @brief Set accuracy of the exp/log accelerator.
//! @see Globalvariable NMU_EXP_LOG_CONTROL
__static_inline void nmu_exp_log_control (uint32_t iterations)
{
    *NMU_EXP_LOG_CONTROL = iterations;
}

//! @brief Floatingpoint version of expf ()
__static_inline float hwexpf (float arg)
{
#ifdef HWEXPF_FP_APPROX
  #ifndef HWEXPF_LIMIT_FP_LOW 
    #define HWEXPF_LIMIT_FP_LOW -2.0
  #endif
  #ifndef HWEXPF_LIMIT_FP_HIGH
    #define HWEXPF_LIMIT_FP_LOW 10.0
  #endif
    if (arg>HWEXPF_LIMIT_FP_LOW && arg<HWEXPF_LIMIT_FP_HIGH) {
        nmu_exp_calc (float2fxpt(arg));
        return fxpt2float (nmu_exp_fetch ());
    }
#endif
    /* 49 cycles in total (inline saves 10 (!) cycles)*/

    /* range checking */
    if (unlikely (arg<-87.3365 /* ln (2^-126) [no denormal numbers] */)) {
        return 0.0;
    }
    if (unlikely (arg>88.7228 /* ln (2^128)  */)) {
        union {
            uint32_t i;
            float f;
        } nan;
        nan.i = 0x7f8fffff; /* NaN */;
        return nan.f;
    }

    const int32_t F_LN = 8;
    const int32_t MIN_V = 0x0002c5c9; /* hwexp (MIN_V)>256 */
    const int32_t MAX_V= 0x00031e81; /* hwexp (MAX_V)<512 */

    const float LN_2 = 0.69314718f;
    const float INV_LN_2 = (1.0/0.69314718f);

    float_word_t a;
    a.f = arg;
    // normalize argument to [F_LN*ln(2), (F_LN+1)*ln(2)]
    int32_t n = (int32_t) (a.f*INV_LN_2);
    n = (a.w&0x80000000) ? n-(F_LN+1) : n-F_LN;
    a.f = a.f - n*LN_2;

    int32_t s = (23/*float-frac*/-15/*fp-frac*/-2/*exp*/);
    //int32_t hwexp_x = (0x00800000|(a.w&0x007fffff)) >> s;
    /* since we already know the exponent(0x81), we can mask and set in one cycle */
    int32_t hwexp_x = (a.w^0x40000000) >> s;

    nmu_exp_calc (hwexp_x);
    int32_t exp = 127+n+F_LN;

    exp<<=23;

    float_word_t res;
    res.w = exp;
    if (unlikely (hwexp_x<MIN_V)) {
        nmu_exp_fetch ();
        res.w |= 0x00000000;
    } else if (unlikely (hwexp_x>MAX_V)) {
        nmu_exp_fetch ();
        res.w |= 0x007fffff;
    } else {
        res.w |= (nmu_exp_fetch ())&0x007fffff;
    }

    return res.f;
}
//! @}

//! @name Random acceleration units
//! @{

//! Selection of the Random Number Generator
typedef enum {
	kiss32_0, //!< 1st KISS32 Random Number Generator
	kiss32_1, //!< 2nd KISS32 Random Number Generator
	kiss64_0, //!<     KISS64 Random Number Generator
#ifndef DOXYGEN
    KISS_COUNT
#endif
} kiss_t;

//! @brief Read Integer value from the hardware KISS
__static_inline uint32_t kiss_read_int (kiss_t kiss)
{
    assert (kiss>=0 && kiss < KISS_COUNT);
    return NMU_KISS[kiss].rnd_value;
}

//! @brief Read Integer value from the hardware KISS
__static_inline float kiss_read_float (kiss_t kiss)
{
    assert (kiss>=0 && kiss < KISS_COUNT);
    return NMU_KISS[kiss].rnd_value_float;
}

//! @brief Write KISS Seed
__static_inline void kiss_seed_set (kiss_t kiss, int i_seed, uint32_t value)
{
    assert (kiss>=0 && kiss < KISS_COUNT);
    assert (i_seed >= 0 && i_seed < 5);
    NMU_KISS[kiss].seed[i_seed] = value;
}

//! @brief Read KISS Seed
__static_inline uint32_t kiss_seed_get (kiss_t kiss, int i_seed)
{
    assert (kiss>=0 && kiss < KISS_COUNT);
    assert (i_seed >= 0 && i_seed < 5);
    return NMU_KISS[kiss].seed[i_seed];
}
//! @}


#ifdef __cplusplus
 }
#endif

#endif
