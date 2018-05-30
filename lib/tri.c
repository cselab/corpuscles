#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/vec.h"
#include "he/tri.h"

real tri_area(real a[3], real b[3], real c[3]) {
    real u[3], v[3], n[3];
    vec_minus(b, a, u);
    vec_minus(c, a, v);
    vec_cross(u, v,   n);
    return vec_abs(n)/2;
}

/* oriented volume of tetrahedral [0, a, b, c] */
real tri_volume(real a[3], real b[3], real c[3]) {
    real n[3];
    vec_cross(a, b,   n);
    return vec_dot(c, n)/6;
}
