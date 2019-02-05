#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "real.h"
#include "he/err.h"

#define SET(key, val) if ((key) != NULL) *(key) = (val)
#define NOT_ZERO(x) if ((x) == 0) ERR(HE_NUM, "should not be zero");

static real sq(real x) { return x*x; }
static real select(real ref, real cur) {
    enum {UNSET, REF, CUR};
    static int f = UNSET;
    if (f == UNSET) {
        if (!getenv("AREA")) {
            f = REF;
            MSG("area: reference");
        } else {
            f = CUR;
            MSG("area: current");
        }
    }
    if (f == REF)
        return ref;
    else
        return cur;
}

#include "he/strain/2d.h"
#include "strain/2d.inc"
