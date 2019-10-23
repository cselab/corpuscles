#include <tgmath.h>
#include <stdio.h>

#include "real.h"

#include "co/err.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/edg.h"
#include "co/dedg.h"
#include "co/dtri.h"
#include "co/ten.h"

/* n x (b - a)/|b - a|^2   */
static int
angle0(const real a[3], const real b[3], const real n[3], /**/ real da[3])
{
    real v0, v[3], nv[3];

    vec_minus(b, a, v);
    vec_cross(n, v, nv);
    v0 = vec_dot(v, v);
    if (v0 == 0)
        ERR(CO_NUM, "v0 == 0");
    vec_scalar(nv, 1 / v0, da);
    return CO_OK;
}

int
dtri_angle(const real a[3], const real b[3], const real c[3], real da[3],
           real db[3], real dc[3])
{
    real n[3], nda[3];

    tri_normal(b, c, a, n);
    angle0(c, b, n, dc);
    angle0(b, a, n, da);
    vec_plus(dc, da, nda);
    vec_scalar(nda, -1, db);
    return CO_OK;
}

int
dtri_cot(const real a[3], const real b[3], const real c[3], real da[3],
         real db[3], real dc[3])
{
    real ang, s, da0[3], db0[3], dc0[3];

    ang = tri_angle(a, b, c);
    s = sin(ang);
    if (s == 0)
        ERR(CO_NUM, "s = 0");
    s = -1 / (s * s);
    dtri_angle(a, b, c, da0, db0, dc0);
    vec_scalar(da0, s, da);
    vec_scalar(db0, s, db);
    vec_scalar(dc0, s, dc);
    return CO_OK;
}

int
dtri_area(const real a[3], const real b[3], const real c[3],
          /**/ real da[3], real db[3], real dc[3])
{
    real n[3], n2[3], ab[3], bc[3], ca[3];

    tri_normal(a, b, c, n);
    vec_scalar(n, 0.5, n2);
    tri_edg(a, b, c, ab, bc, ca);
    vec_cross(n2, bc, da);
    vec_cross(n2, ca, db);
    vec_cross(n2, ab, dc);
    return CO_OK;
}

static int
dvolume(const real a[3], const real b[3], const real c[3], /**/ real dd[3])
{
    real n[3], area;

    tri_normal(a, b, c, /**/ n);
    area = tri_area(a, b, c);
    vec_scalar(n, area / 3, /**/ dd);
    return CO_OK;
}

int
dtri_volume(const real a[3], const real b[3], const real c[3],
            /**/ real da[3], real db[3], real dc[3])
{
    real z[3];

    vec_zero(z);
    dvolume(z, a, b, /**/ dc);
    dvolume(z, b, c, /**/ da);
    dvolume(z, c, a, /**/ db);
    return CO_OK;
}

static int
normal0(const real a[3], const real n[3], /**/ Ten * t)
{
    real u[3];

    vec_cross(a, n, u);
    ten_dyadic(u, n, t);
    return CO_OK;
}

#define NOT_ZERO(x) if ((x) == 0) ERR(CO_NUM, "should not be zero");
int
dtri_normal(const real a[3], const real b[3], const real c[3],
            /**/ Ten * x, Ten * y, Ten * z)
{
    real n[3], A, u[3], v[3], w[3], coef;

    tri_normal(a, b, c, n);
    A = tri_area(a, b, c);

    vec_minus(c, b, u);
    vec_minus(a, c, v);
    vec_minus(b, a, w);

    normal0(u, n, x);
    normal0(v, n, y);
    normal0(w, n, z);

    NOT_ZERO(A);
    coef = 1 / (2 * A);

    ten_scale(coef, x);
    ten_scale(coef, y);
    ten_scale(coef, z);

    /* TODO */
    return CO_OK;
}

int
dtri_edg_area(const real a[3], const real b[3], const real c[3],
              /**/ real da[3], real db[3], real dc[3])
{
    real A, s;
    real Aa[3], Ab[3], Ac[3];
    real sa[3], sb[3];

    A = tri_area(a, b, c);
    NOT_ZERO(A);
    s = edg_sq(a, b) / (A * A);

    dtri_area(a, b, c, /**/ Aa, Ab, Ac);
    dedg_sq(a, b, /**/ sa, sb);
    vec_linear_combination(1 / A, sa, -s, Aa, /**/ da);
    vec_linear_combination(1 / A, sb, -s, Ab, /**/ db);
    vec_scalar(Ac, -s, /**/ dc);

    return CO_OK;
}

int
dtri_alpha(const real a[3], const real b[3], const real c[3],
           const real u[3], const real v[3], const real w[3],
           /**/ real du[3], real dv[3], real dw[3])
{
    real A, s;

    A = tri_area(a, b, c);
    NOT_ZERO(A);
    dtri_area(u, v, w, du, dv, dw);
    s = 1 / A;
    vec_scale(s, du);
    vec_scale(s, dv);
    vec_scale(s, dw);

    return CO_OK;
}

/* diff(be, v), diff(be, w) */
static real
be_v(real b, real c, real v, real w)
{
    return (c - (sqrt(b * c - 4) * w) / sqrt(v * w - 4)) / 8;
}

static real
be_w(real b, real c, real v, real w)
{
    return (b - (sqrt(b * c - 4) * v) / sqrt(v * w - 4)) / 8;
}

int
dtri_beta(const real a[3], const real b0[3], const real c0[3],
          const real u[3], const real v0[3], const real w0[3],
          /**/ real du[3], real dv[3], real dw[3])
{
    real b, c, v, w;
    real vu[3], vv[3], vw[3], wu[3], wv[3], ww[3];
    real bv, bw;

    b = tri_edg_area(a, b0, c0);
    c = tri_edg_area(a, c0, b0);
    v = tri_edg_area(u, v0, w0);
    w = tri_edg_area(u, w0, v0);
    dtri_edg_area(u, v0, w0, vu, vv, vw);
    dtri_edg_area(u, w0, v0, wu, ww, wv);
    bv = be_v(b, c, v, w);
    bw = be_w(b, c, v, w);
    vec_linear_combination(bv, vu, bw, wu, /**/ du);
    vec_linear_combination(bv, vv, bw, wv, /**/ dv);
    vec_linear_combination(bv, vw, bw, ww, /**/ dw);

    return CO_OK;
}

int
dtri_lim_area(real Ka, real a3, real a4, const real a[3], const real b[3],
              const real c[3], const real u[3], const real v[3],
              const real w[3], real du[3], real dv[3], real dw[3])
{
    real l, s;

    l = tri_alpha(a, b, c, u, v, w);
    s = (Ka * (4 * a4 * l * l * l + 3 * a3 * l * l + 2 * l)) / 2;
    dtri_alpha(a, b, c, u, v, w, du, dv, dw);
    vec_scale(s, du);
    vec_scale(s, dv);
    vec_scale(s, dw);
    return CO_OK;
}

int
dtri_lim_shear(real mu, real b1, real b2, const real a[3], const real b[3],
               const real c[3], const real u[3], const real v[3],
               const real w[3], real du[3], real dv[3], real dw[3])
{
    real al, be, dal, dbe;
    real al_du[3], al_dv[3], al_dw[3];
    real be_du[3], be_dv[3], be_dw[3];

    al = tri_alpha(a, b, c, u, v, w);
    be = tri_beta(a, b, c, u, v, w);

    dal = b1 * be * mu;
    dbe = (2 * b2 * be + al * b1 + 1) * mu;

    dtri_alpha(a, b, c, u, v, w, al_du, al_dv, al_dw);
    dtri_beta(a, b, c, u, v, w, be_du, be_dv, be_dw);

    vec_linear_combination(dal, al_du, dbe, be_du, /**/ du);
    vec_linear_combination(dal, al_dv, dbe, be_dv, /**/ dv);
    vec_linear_combination(dal, al_dw, dbe, be_dw, /**/ dw);

    return CO_OK;
}

int
dtri_lim(real Ka, real a3, real a4, real mu, real b1, real b2,
         const real a[3], const real b[3], const real c[3],
         const real u[3], const real v[3], const real w[3],
         /**/ real du[3], real dv[3], real dw[3])
{
    real ar_du[3], ar_dv[3], ar_dw[3];
    real sh_du[3], sh_dv[3], sh_dw[3];

    dtri_lim_area(Ka, a3, a4, a, b, c, u, v, w, ar_du, ar_dv, ar_dw);
    dtri_lim_shear(mu, b1, b2, a, b, c, u, v, w, sh_du, sh_dv, sh_dw);

    vec_plus(ar_du, sh_du, du);
    vec_plus(ar_dv, sh_dv, dv);
    vec_plus(ar_dw, sh_dw, dw);
    return CO_OK;
}
