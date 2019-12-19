#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "real.h"
#include "co/err.h"

#define SET(key, val) if ((key) != NULL) *(key) = (val)
#define NOT_ZERO(x) if ((x) == 0) ERR(CO_NUM, "should not be zero");

static real
sq(real x)
{
    return x * x;
}

#include "co/strain/2d.h"
#include "strain/2d.inc"
