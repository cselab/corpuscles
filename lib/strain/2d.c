#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/err.h"

#define SET(key, val) if ((key) != NULL) *(key) = (val)
#define NOT_ZERO(x) if ((x) == 0) ERR(HE_NUM, "should not be zero");

static real sq(real x) { return x*x; }
static real select(real x, real y) {
    return x;
}

#include "he/strain/2d.h"
#include "strain/2d.inc"
