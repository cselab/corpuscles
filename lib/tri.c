#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
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

int tri_normal(real a[3], real b[3], real c[3], /**/ real e[3]) {
    real u[3], v[3], n[3];
    vec_minus(b, a,   u);
    vec_minus(c, a,   v);
    vec_cross(u, v,   n);
    vec_norm(n,   e);
    return HE_OK;
}

real tri_angle(real a[3], real b[3], real c[3]) { /* at `b' */
    real u[3], v[3];
    vec_minus(a, b, u);
    vec_minus(c, b, v);
    return vec_angle(u, v);
}

real tri_cot(real a[3], real b[3], real c[3]) { /* at `b' */
    real x, y, u[3], v[3];
    y = 2 * tri_area(a, b, c);
    vec_minus(a, b, u);
    vec_minus(c, b, v);
    x = vec_dot(u, v);
    return x/y; /* TODO: */
}

/* bc is an edge */
real tri_dih(real a[3], real b[3], real c[3], real d[3]) {
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

int tri_edg(real a[3], real b[3], real c[3], /**/ real ab[3], real bc[3], real ca[3]) {
    vec_minus(b, a,   ab);
    vec_minus(c, b,   bc);
    vec_minus(a, c,   ca);
    return HE_OK;
}
