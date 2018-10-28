#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/err.h"

#define SET(key, val) if ((key) != NULL) *(key) = (val)
#define NOT_ZERO(x) if ((x) == 0) ERR(HE_NUM, "should not be zero");
static real sq(real x) { return x*x; }

#include "he/linear_strain_tri.h"
#include "linear_strain_tri.inc"
