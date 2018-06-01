#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

static void angle0(real a[3], real b[3], real n[3],
                   real da[3]) {
    real v0, v[3], nv[3];
    vec_minus(b, a,  v);
    vec_cross(n, v, nv);
    v0 = vec_dot(v, v); /* TODO: */
    vec_scalar(nv, 1/v0,  da);
}
