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

static real
select0(real ref, real cur)
{
    enum { UNSET, YES, NO };
    static int f = UNSET;

    if (f == UNSET) {
        if (getenv("AREA"))
            f = YES;
        else
            f = NO;
    }
    return f == YES ? cur : ref;
}

#include "co/strain/2d.h"
#include "strain/2d.inc"
