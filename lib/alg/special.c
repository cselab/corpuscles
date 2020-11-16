#include <gsl/gsl_sf_bessel.h>

#include <real.h>
#include "alg/special.h"

real
alg_special_bessel_J0(real x)
{
    return gsl_sf_bessel_J0(x);
}

real
alg_special_bessel_J1(real x)
{
    return gsl_sf_bessel_J1(x);
}

real
alg_special_bessel_J2(real x)
{
    return gsl_sf_bessel_Jn(2, x);
}

real
alg_special_bessel_Jn(int n, real x)
{
    return gsl_sf_bessel_Jn(n, x);
}
