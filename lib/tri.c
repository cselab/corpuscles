#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/macro.h"

#include "he/tri.h"

static void swap(real *a, real *b) {
    double t;
    t = *a; *a = *b; *b = t;
}
static int less(real *a, real *b) { return (*a) < (*b); }
static void sort3(real *a, real *b, real *c) {
    if (less(c, b)) swap(c, b);
    if (less(b, a)) swap(b, a);
    if (less(c, b)) swap(c, b);
}

static real kahan_area0(real a, real b, real c) {
    sort3(&c, &b, &a); /* make a > b > c */
    return sqrt((a+(b+c))*(c-(a-b))*(c+(a-b))*(a+(b-c)))/4;
}

static real kahan_area(const real r0[3], const real r1[3], const real r2[3]) {
    real r01[3], r12[3], r20[3], a, b, c;
    vec_minus(r0, r1, /**/ r01);
    vec_minus(r1, r2, /**/ r12);
    vec_minus(r2, r0, /**/ r20);
    a = vec_abs(r01); b = vec_abs(r12); c = vec_abs(r20);
    return kahan_area0(a, b, c);
}

static real __UNUSED naive_area(const real a[3], const real b[3], const real c[3]) {
    real u[3], v[3], n[3];
    vec_minus(b, a, u);
    vec_minus(c, a, v);
    vec_cross(u, v,   n);
    return vec_abs(n)/2;
}

real tri_area(const real a[3], const real b[3], const real c[3]) {
    return kahan_area(a, b, c);
}

/* oriented volume of tetrahedral [0, a, b, c] */
real tri_volume(const real a[3], const real b[3], const real c[3]) {
    real n[3];
    vec_cross(a, b,   n);
    return vec_dot(c, n)/6;
}

int tri_normal(const real a[3], const real b[3], const real c[3], /**/ real e[3]) {
    real u[3], v[3], n[3];
    vec_minus(b, a,   u);
    vec_minus(c, a,   v);
    vec_cross(u, v,   n);
    vec_norm(n,   e);
    return HE_OK;
}

real tri_angle(const real a[3], const real b[3], const real c[3]) { /* at `b' */
    real u[3], v[3];
    vec_minus(a, b, u);
    vec_minus(c, b, v);
    return vec_angle(u, v);
}

#define NOT_ZERO(x) if ((x) == 0) ERR(HE_NUM, "should not be zero");
real tri_cot(const real a[3], const real b[3], const real c[3]) { /* at `b' */
    real x, y, u[3], v[3];
    y = 2 * tri_area(a, b, c);
    vec_minus(a, b, u);
    vec_minus(c, b, v);
    x = vec_dot(u, v);
    NOT_ZERO(y);
    return x/y;
}

int tri_edg(const real a[3], const real b[3], const real c[3], /**/ real ab[3], real bc[3], real ca[3]) {
    vec_minus(b, a,   ab);
    vec_minus(c, b,   bc);
    vec_minus(a, c,   ca);
    return HE_OK;
}

int tri_3to2(const real a[3], const real b[3], const real c[3],
             /**/ real *ux, real *uy, real *wx, real *wy) {
    real ab[3], ac[3];
    vec_minus(b, a, /**/ ab);
    vec_minus(c, a, /**/ ac);

    *ux  = vec_abs(ab);
    *uy  = 0;
    *wx = vec_project_scalar(ac, ab);
    *wy = vec_reject_scalar(ac, ab);
    return HE_OK;
}

int tri_2to3(const real a[3], const real b[3], const real c[3], /**/ real ex[3], real ey[3]) {
    real ab[3], ac[3], r[3];
    vec_minus(b, a, /**/ ab);
    vec_minus(c, a, /**/ ac);

    vec_norm(ab, /**/ ex);
    vec_reject(ac, ab, /**/ r);
    vec_norm(r, /**/ ey);

    return HE_OK;
}
