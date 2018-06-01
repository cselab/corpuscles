#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

/* n x (b - a)/|b - a|^2   */
static int angle0(real a[3], real b[3], real n[3],
                   real da[3]) {
    real v0, v[3], nv[3];
    vec_minus(b, a,  v);
    vec_cross(n, v, nv);
    v0 = vec_dot(v, v); /* TODO: */
    vec_scalar(nv, 1/v0,  da);
    return HE_OK;
}

int dtri_angle0(real a[3], real b[3], real c[3],   real da[3], real db[3], real dc[3]) {
    real ab[3], ac[3], n[3], nda[3];
    tri_normal(b, c, a,   n);
    angle0(c, b, n,   dc);
    angle0(b, a, n,   da);
    vec_plus(dc, da,  nda);
    vec_negative(nda,    db);
    return HE_OK;
}
