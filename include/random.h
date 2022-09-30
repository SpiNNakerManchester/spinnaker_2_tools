
#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>

#include "attributes.h"
#include "communication.h"

#include "nmu.h"

#ifndef INIT_KISS_X
#define INIT_KISS_X 143185431
#endif

#ifndef INIT_KISS_Y
#define INIT_KISS_Y 987654321
#endif

#ifndef INIT_KISS_Z
#define INIT_KISS_Z 1762089
#endif

#ifndef INIT_KISS_C
#define INIT_KISS_C 6543217 /* Seed variables */
#endif

#ifdef USE_SOFTWARE_RNG
//#pragma message "Using Software Random Number Generator"
#define RNG_WRAPPER(func) func##_soft
#else
#define RNG_WRAPPER(func) func##_hard
#endif

typedef uint32_t (*rng_uni)  (void);
typedef float    (*rng_unif) (void);
extern uint32_t kiss_x, kiss_y, kiss_z, kiss_c;

//============  srand functions  ===============================================

__static_inline void srand_soft (void)
{
	uint32_t unused __attribute__((unused));

	kiss_x = INIT_KISS_X * (getMyChipID () + getMyPEID () + 1);
	kiss_y = INIT_KISS_Y * (getMyChipID () + getMyPEID () + 1);
	kiss_z = INIT_KISS_Z * (getMyChipID () + getMyPEID () + 1);
	kiss_c = INIT_KISS_C * (getMyChipID () + getMyPEID () + 1);
}

__static_inline void srand_hard (void)
{
	uint32_t unused __attribute__((unused));

	kiss_seed_set (kiss64_0, 1, INIT_KISS_X * (getMyChipID () + getMyPEID () + 1));
	kiss_seed_set (kiss64_0, 2, INIT_KISS_Y * (getMyChipID () + getMyPEID () + 1));
	kiss_seed_set (kiss64_0, 3, INIT_KISS_Z * (getMyChipID () + getMyPEID () + 1));
	kiss_seed_set (kiss64_0, 0, INIT_KISS_C * (getMyChipID () + getMyPEID () + 1));
}

__static_inline void srand ()
{
	RNG_WRAPPER (srand) ();
}


//============  integer uni_rand functions  ====================================


__static_inline uint32_t uni_rand_soft (void)
{
	// mars_kiss64_simp:
    uint64_t t;
#ifdef STANDARD_SOFTWARE_KISS
	// instead of
    kiss_x = 314527869 * kiss_x + 1234567;
#else
	// hardware kiss_calculates:
    kiss_x = 304042114 * kiss_x + 1234567;
#endif

    kiss_y ^= kiss_y << 5;
    kiss_y ^= kiss_y >> 7;
    kiss_y ^= kiss_y << 22;
    t = 4294584393ULL * kiss_z + kiss_c;
    kiss_c = t >> 32;
    kiss_z = t;

    return ((uint32_t)kiss_x + kiss_y + kiss_z);
}

__static_inline uint32_t uni_rand_hard (void)
{
	return kiss_read_int (kiss64_0);
}

__static_inline uint32_t uni_rand (void)
{
	return RNG_WRAPPER (uni_rand) ();
}

//============  floating-point uni_randf functions  ============================
__static_inline float rng_uni_randf (const rng_uni rng_uniform)
{
	const float uint32_max = 4294967295.0f;
	return ((float)(*rng_uniform) ()) / uint32_max;
}

__static_inline float uni_randf_soft (void)
{
	return rng_uni_randf (&uni_rand_soft);
}

__static_inline float uni_randf_hard (void)
{
	return rng_uni_randf (&uni_rand_hard);
}

__static_inline float uni_randf (void)
{
	return rng_uni_randf (&RNG_WRAPPER (uni_rand));
}


//============  gaussian distribution  =========================================
float rng_gauss (rng_unif rng_uniform);

__static_inline float gauss_randf (void)
{
#ifdef FAST_GAUSS
    extern const float invcdf[257];
	union {
        struct {
            uint8_t v;
            int8_t s;
        };
		int16_t r;
	} v;

    v.r = uni_rand ();
	float l = invcdf[v.v];
	float d = invcdf[v.v+1] - l;
    l += d * (RNG_WRAPPER (uni_randf) ());

	return (v.s & (1<<4)) ? l : -l;
#else
	return rng_gauss (&RNG_WRAPPER (uni_randf));
#endif
}

//============  gamma distribution  ============================================
float rng_gamma (rng_unif rng_uniform, const float a, const float b);
__static_inline float gamma_randf (const float a, const float b)
{
	return rng_gamma (&RNG_WRAPPER (uni_randf), a, b);
}

#endif /* __RANDOM_H__ */

