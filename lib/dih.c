#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"

#include "he/dih.h"

real dih_angle(real a[3], real b[3], real c[3], real d[3]) {
    real x, y, ang, n[3], k[3], nk[3], e[3];
    tri_normal(b, c, a,   n);
    tri_normal(c, b, d,   k);
    x = vec_dot(n, k);
    vec_cross(n, k,    nk);
    y = vec_abs(nk);
    ang = atan2(y, x);
    vec_minus(c, b, e);
    if (vec_dot(e, nk) < 0)
        ang = - ang;
    return ang;
}


