
#include <nmu.h>

#define F_LN 8
#define MIN_V 0x0002c5c9 /* hwexp (MIN_V)>256 */
#define MAX_V 0x00031e81 /* hwexp (MAX_V)<512 */

#define LN_2 (0.69314718f)
#define INV_LN_2 (1.0/0.69314718f)

float _hwexpf (float arg)
{
    float_word_t a;
    a.f = arg;
    // normalize argument to [F_LN*ln(2), (F_LN+1)*ln(2)]
    int32_t n = (int32_t) (a.f*INV_LN_2);
    n = (a.w&0x80000000) ? n-(F_LN+1) : n-F_LN;
    a.f = a.f - n*LN_2;

    int32_t s = (23/*float-frac*/-15/*fp-frac*/-2/*exp*/);
    //int32_t hwexp_x = (0x00800000|(a.w&0x007fffff)) >> s;
    int32_t hwexp_x = (a.w^0x40800000) >> s;

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

