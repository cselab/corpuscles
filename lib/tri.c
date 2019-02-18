#include <tgmath.h>
#include <stdio.h>

#include "real.h"

#include "he/err.h"
#include "he/edg.h"
#include "he/vec.h"
#include "he/macro.h"

#include "he/tri.h"

#define FMT   HE_REAL_OUT

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
    real s;
    sort3(&c, &b, &a); /* make a > b > c */
    s = (a+(b+c))*(c-(a-b))*(c+(a-b))*(a+(b-c));
    if (s < 0)
        ERR(HE_NUM, "s < 0: a = %g, b = %g, c = %g", a, b, c);
    return sqrt(s)/4;
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
    return fabs(vec_angle(u, v));
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

int tri_center(const real a[3], const real b[3], const real c[3], /**/ real center[3]) {
    return vec_mean3(a, b, c, /**/ center);
}

int tri_off(const real a[3], const real b[3], const real c[3], FILE *f) {
    int status;
    status = fputs("OFF\n"
                   "3 1 0\n", f);
    if (status == EOF)
        ERR(HE_IO, "fail to write");
    vec_fprintf(a, f, FMT);
    vec_fprintf(b, f, FMT);
    vec_fprintf(c, f, FMT);
    fputs("3 0 1 2\n", f);
    return HE_OK;
}

int tri_vect(const real a[3], const real b[3], const real c[3],
             const real u[3], const real v[3], const real w[3],
             FILE *f) {
    int status;
    real x[3], y[3], z[3];
    vec_plus(a, u, x);
    vec_plus(b, v, y);
    vec_plus(c, w, z);

    status = fputs("VECT\n"
                   "3 6 0\n"
                   "2 2 2\n"
                   "0 0 0\n", f);
    if (status == EOF)
        ERR(HE_IO, "fail to write");
    vec_fprintf(a, f, FMT);
    vec_fprintf(x, f, FMT);

    vec_fprintf(b, f, FMT);
    vec_fprintf(y, f, FMT);

    vec_fprintf(c, f, FMT);
    vec_fprintf(z, f, FMT);

    return HE_OK;
}

int tri_list(const real a[3], const real b[3], const real c[3],
             const real u[3], const real v[3], const real w[3],
             FILE *f) {
    if (fputs("LIST\n", f) == EOF)
        ERR(HE_IO, "fail to write");
    fputs("{\n", f);
    if (tri_off(a, b, c, f) != HE_OK)
        ERR(HE_IO, "tri_vect failed");
    fputs("}\n", f);
    fputs("{\n", f);
    if (tri_vect(a, b, c, u, v, w, f) != HE_OK)
        ERR(HE_IO, "tri_vect failed");
    fputs("}\n", f);
    return HE_OK;
}

int tri_3to2(const real a[3], const real b[3], const real c[3], /**/ real *ux, real *wx, real *wy) {
    real u[3], v[3], n[3], ey[3], nx[3], ny[3];
    vec_minus(b, a, /**/ u);
    vec_minus(c, a, /**/ v);

    vec_norm(u, nx);
    vec_cross(u, v,   n);
    vec_cross(n, u,  ey);
    vec_norm(ey, ny);

    *ux = vec_dot(u, nx);
    *wx = vec_dot(v, nx);
    *wy = vec_dot(v, ny);
    return HE_OK;
}

int tri_2to3(const real a[3], const real b[3], const real c[3], /**/ real nx[3], real ny[3]) {
    real u[3], v[3], n[3], ey[3];
    vec_minus(b, a, /**/ u);
    vec_minus(c, a, /**/ v);

    vec_norm(u, nx);
    vec_cross(u, v,   n);
    vec_cross(n, u,  ey);
    vec_norm(ey, ny);

    return HE_OK;
}

int tri_2d_invariants(real bx, real cx, real cy, real ux, real wx, real wy, /**/ real *al, real *be) {
#   define sq(x) ((x)*(x))
#   define SET(key, val) if ((key) != NULL) *(key) = (val)
    real px, py, qy;
    NOT_ZERO(bx);
    NOT_ZERO(cy);

    px = ux/bx;
    py = -(cx*ux-bx*wx)/(bx*cy);
    qy = wy/cy;

    SET(al, px*qy-1);
    SET(be, (sq(qy)-2*px*qy+sq(py)+sq(px))/(2*px*qy));

    return HE_OK;
#   undef sq
#   undef SET
}

int tri_3d_invariants(const real a[3], const real b[3], const real c[3], const real u[3], const real v[3], const real w[3], /**/ real *al, real *be) {
    real i, jx, jy;
    real x, yx, yy;

    tri_3to2(a, b, c, /**/ &i, &jx, &jy);
    tri_3to2(u, v, w, /**/ &x, &yx, &yy);

    return tri_2d_invariants(i, jx, jy,   x, yx, yy, /**/ al, be);
}

real tri_edg_area(const real a[3], const real b[3], const real c[3]) {
    real A, s;
    s = edg_sq(a, b);
    A = tri_area(a, b, c);
    NOT_ZERO(A);
    return s/A;
}

real tri_alpha(const real a[3], const real b[3], const real c[3], const real u[3], const real v[3], const real w[3]) {
    real A, B;
    A = tri_area(a, b, c);
    B = tri_area(u, v, w);
    NOT_ZERO(A);
    return B/A - 1;
}

static real be(real b, real c, real v, real w) { return -(2*sqrt(b*c-4)*sqrt(v*w-4)-b*w-c*v+8)/8; }
real tri_beta(const real a[3], const real b0[3], const real c0[3], const real u[3], const real v0[3], const real w0[3]) {
    real b, c, v, w;
    b = tri_edg_area(a, b0, c0);
    c = tri_edg_area(a, c0, b0);
    v = tri_edg_area(u, v0, w0);
    w = tri_edg_area(u, w0, v0);
    return be(b, c, v, w);
}

real tri_lim_area(real Ka, real a3, real a4, const real a[3], const real b[3], const real c[3], const real u[3], const real v[3], const real w[3]) {
    real al;

    al = tri_alpha(a, b, c, u, v, w);
    return Ka/2*(al*al + a3*al*al*al + a4*al*al*al*al);
}

real tri_lim_shear(real mu, real b1, real b2, const real a[3], const real b[3], const real c[3], const real u[3], const real v[3], const real w[3]) {
    real al, be;

    al = tri_alpha(a, b, c, u, v, w);
    be = tri_beta(a, b, c, u, v, w);
    return mu*(be + b1*al*be + b2*be*be);
}

real tri_lim(real Ka, real a3, real a4, real mu, real b1, real b2,
             const real a[3], const real b[3], const real c[3], const real u[3], const real v[3], const real w[3]) {
    return tri_lim_area(Ka, a3, a4, a, b, c, u, v, w) + tri_lim_shear(mu, b1, b2, a, b, c, u, v, w);
}
