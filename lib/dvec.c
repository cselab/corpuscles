#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/ten.h"
#include "he/vec.h"
#include "he/dvec.h"

enum {X, Y, Z};

int dvec_norm(real a[3], Ten *t) {
    real r, n[3];
    Ten nn;
    r = vec_abs(a);
    if (r == 0)
        ERR(HE_NUM, "r == 0");

    vec_norm(a, n);
    ten_one(/**/ t);
    ten_dyadic(n, n, &nn);
    ten_sub(&nn, t);
    ten_scale(1/r, t);
    return HE_OK;
}
