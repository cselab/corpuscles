#include <math.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include <alg/integration.h>
#include <alg/special.h>
#include "co/err.h"
#include "co/green/3wall2.h"
#include "co/he.h"
#include "co/i/vec.h"
#include "co/i/tri.h"
#include "co/i/area.h"
#include "co/i/matrix.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/ten.h"
#include "co/tensor3x3.h"

#define T Green3Wall2
enum { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };
enum {
    X, Y, Z
};
static const real pi = 3.141592653589793115997964;
enum { INTEGRATION_TYPE = QAGS };
static const real qmax = 100;

struct Param {
    real(*j) (real);
    real s;
    real z;
    real z0;
    real W;
     real(*f) (real, real, real, real);
};

struct Param0 {
    real z;
    real W;
     real(*f) (real, real, real);
};

struct Input {
    real x;
    real y;
    real z;
    real z0;
    real W;
};

struct Input0 {
    real z;
    real W;
};

struct T {
    real w;
    real *nx, *ny, *nz, *area;
    AlgIntegration *integration;
};

static real sq(real);
static int d0(T *, const real[3], const real[3], Ten *);
static real F(real, void *);
static real F0(real, void *);
static real fAp(real);
static real fAm(real);
static real fBp(real);
static real fBm(real);
static real fCp(real);
static real fCm(real);
static real fDp(real);
static real fDm(real);
static real fEp(real);
static real fEm(real);
static real tnn(real q, real z, real z0, real W);
static real tnp(real q, real z, real z0, real W);
static real tpn(real q, real z, real z0, real W);
static real tpp(real q, real z, real z0, real W);
static real rpp(real q, real z, real z0, real W);
static real tnn0(real q, real z, real W);
static real tpp0(real q, real z, real W);
static real rpp0(real q, real z, real W);
static int f_xx_yy(struct Input *, AlgIntegration *, real *, real *);
static int f_zz(struct Input *, AlgIntegration *, real *);
static int f_xy(struct Input *, AlgIntegration *, real *);
static int f_xz_yz(struct Input *, AlgIntegration *, real * xz, real * yz);
static int f_zx_zy(struct Input *i, AlgIntegration *, real * zx,
                   real * zy);
static int f_xx_yy0(struct Input0 *, AlgIntegration *, real *, real *);
static int f_zz0(struct Input0 *, AlgIntegration *, real *);
static real xcoth(real);

int
green3_wall2_ini(He * he, real w, T ** pq)
{
    T *q;
    real *nx, *ny, *nz, *area;
    int n;

    MALLOC(1, &q);
    n = he_nv(he);
    MALLOC3(n, &nx, &ny, &nz);
    MALLOC(n, &area);
    q->w = w;
    q->nx = nx;
    q->ny = ny;
    q->nz = nz;
    q->area = area;
    if (alg_integration_ini(INTEGRATION_TYPE, &q->integration) != CO_OK)
        ERR(CO_MEMORY, "alg_integration_ini failed");
    *pq = q;
    return CO_OK;
}

int
green3_wall2_fin(T * q)
{
    FREE3(q->nx, q->ny, q->nz);
    FREE(q->area);
    alg_integration_fin(q->integration);
    FREE(q);
    return CO_OK;
}

int
green3_wall2_apply(T * q, He * he, const real * x, const real * y,
                   const real * z, struct Tensor3x3 *t)
{
    int n;
    int i;
    int k;
    real s;

    n = he_nv(he);
    tensor3x3_zero(n, t);
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        Ten t0;
        real a[3], b[3];
        int j;

        i_vec_get(i, x, y, z, a);
        for (j = 0; j < n; j++) {
            if (i == j)
                green3_wall2_s0(q, a, &t0);
            else {
                i_vec_get(j, x, y, z, b);
                if (green3_wall2_s(q, a, b, &t0) != CO_OK)
                    ERR(CO_NUM, "green3_wall2_s failed (i=%d, j=%d)", i,
                        j);
            }
            for (k = 0; k < 3 * 3; k++)
                i_matrix_set(n, n, i, j, t0.t[k], t->d[k]);
        }
    }
    s = 1 / (8 * pi);
    tensor3x3_scale(n, s, t);
    return CO_OK;
}

int
green3_wall2_stresslet(T * q, He * he, const real * x, const real * y,
                       const real * z, struct Tensor3x3 *t)
{
    int n;
    int i;
    int k;
    int status;
    real *nx;
    real *ny;
    real *nz;
    real *area;
    real A;

    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    area = q->area;
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
        ERR(CO_NUM, "normal_mwa failed");
    status = i_area_ver(he, x, y, z, area);
    if (status != CO_OK)
        ERR(CO_NUM, "area_ver failed");
    n = he_nv(he);
    tensor3x3_zero(n, t);
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        int j;
        real a[3], b[3], u[3];
        Ten t0;

        i_vec_get(i, x, y, z, a);
        for (j = 0; j < n; j++) {
            A = 3 * area[j] / (4 * pi);
            i_vec_get(j, nx, ny, nz, u);
            if (i == j) {
                d0(q, a, u, &t0);
            } else {
                i_vec_get(j, x, y, z, b);
                if (green3_wall2_t(q, a, u, b, &t0) != CO_OK)
                    ERR(CO_NUM, "green3_wall2_s failed (i=%d, j=%d)", i,
                        j);
            }
            for (k = 0; k < 3 * 3; k++)
                i_matrix_set(n, n, i, j, A * t0.t[k], t->d[k]);
        }
    }
    return CO_OK;
}

static real
rad(real x, real y, real z)
{
    real r;
    real eps;

    eps = 1e-32;
    r = x * x + y * y + z * z;
    if (r < eps)
        ERR(CO_NUM, "r < eps");
    return sqrt(r);
}

int
green3_wall2_s0(T * q, const real a[3], Ten * t0)
{
    real W;
    real *t;
    struct Input0 i;
    AlgIntegration *integration;

    t = t0->t;
    integration = q->integration;
    W = q->w;

    i.z = a[Z];
    i.W = W;
    f_xx_yy0(&i, integration, &t[XX], &t[YY]);
    f_zz0(&i, integration, &t[ZZ]);

    /* walls */
    t[XY] = t[YX] = t[XZ] = t[ZX] = t[YZ] = t[ZY] = 0;
    return CO_OK;
}

static int
d0(T * q, const real a[3], const real n[3], Ten * t0)
{
    real aw;
    real nx;
    real ny;
    real nz;
    real *t;
    real w;

    t = t0->t;
    w = q->w - a[Z];
    aw = fabs(w);
    nx = n[X];
    ny = n[Y];
    nz = n[Z];

    t[XX] = nz / (8 * aw * w);
    t[XY] = 0;
    t[XZ] = nx / (8 * aw * w);
    t[YX] = 0;
    t[YY] = nz / (8 * aw * w);
    t[YZ] = ny / (8 * aw * w);
    t[ZX] = nx / (8 * aw * w);
    t[ZY] = ny / (8 * aw * w);
    t[ZZ] = nz / (2 * aw * w);
    return CO_OK;
}

int
green3_wall2_s(T * q, const real a[3], const real b[3], Ten * t0)
{
    real W;
    real *t;
    real u;
    real v;
    real w;
    real uu;
    real uv;
    real uw;
    real vv;
    real vw;
    real ww;
    real r1;
    real r2;
    real r3;
    struct Input i;
    AlgIntegration *integration;

    t = t0->t;
    integration = q->integration;
    W = q->w;

    u = a[X] - b[X];
    v = a[Y] - b[Y];
    w = a[Z] - b[Z];
    r2 = u * u + v * v + w * w;
    if (r2 == 0)
        ERR(CO_NUM, "r2 == 0");
    r1 = 1 / sqrt(r2);
    r3 = r1 / r2;

    /* walls */
    i.x = a[X] - b[X];
    i.y = a[Y] - b[Y];
    i.z = a[Z];
    i.z0 = b[Z];
    i.W = W;
    f_xx_yy(&i, integration, &t[XX], &t[YY]);
    f_zz(&i, integration, &t[ZZ]);
    f_xy(&i, integration, &t[XY]);
    f_xz_yz(&i, integration, &t[XZ], &t[YZ]);
    f_zx_zy(&i, integration, &t[ZX], &t[ZY]);
    t[YX] = t[XY];

    /* free */
    uu = r1 + u * u * r3;
    vv = r1 + v * v * r3;
    ww = r1 + w * w * r3;
    uv = u * v * r3;
    uw = u * w * r3;
    vw = v * w * r3;
    t[XX] += uu;
    t[XY] += uv;
    t[XZ] += uw;
    t[YX] += uv;
    t[YY] += vv;
    t[YZ] += vw;
    t[ZX] += uw;
    t[ZY] += vw;
    t[ZZ] += ww;

    return CO_OK;
}

int
green3_wall2_t(T * q, const real a[3], const real n[3], const real b[3],
               Ten * t0)
{
    real d[3];
    real nx;
    real ny;
    real nz;
    real r0;
    real r05;
    real r1;
    real r15;
    real r17;
    real *t;
    real w;
    real w2;
    real x;
    real x2;
    real x3;
    real y;
    real y2;
    real y3;
    real z;
    real z2;
    real z3;
    real zw;
    real zw2;
    real zw3;

    t = t0->t;
    w = q->w - b[Z];
    i_vec_minus(a, b, d);

    x = d[X];
    y = d[Y];
    z = d[Z];
    nx = n[X];
    ny = n[Y];
    nz = n[Z];

    zw = z - 2 * w;
    r0 = 1 / rad(x, y, z);
    r1 = 1 / rad(x, y, zw);
    r15 = r1 * r1 * r1 * r1 * r1;
    r17 = r1 * r1 * r1 * r1 * r1 * r1 * r1;
    r05 = r0 * r0 * r0 * r0 * r0;
    w2 = w * w;
    x2 = x * x;
    x3 = x * x * x;
    y2 = y * y;
    y3 = y * y * y;
    z2 = z * z;
    z3 = z * z * z;
    zw2 = zw * zw;
    zw3 = zw * zw * zw;
    t[XX] =
        zw * (2 * w * (r15 - 5 * r17 * x2) * (nz * zw + ny * y) -
              10 * nx * r17 * w * x3 - nz * (10 * r17 * w2 + r15) * x2 +
              2 * r15 * (2 * nx * w * x + nz * w2)) + nz * r05 * x2 * z +
        ny * (2 * r15 * w2 - (10 * r17 * w2 + r15 - r05) * x2) * y +
        nx * (6 * r15 * w2 * x - (10 * r17 * w2 + r15 - r05) * x3);
    t[XY] =
        (-10 * nz * r17 * w * x * y * zw2) - (10 * ny * r17 * w * x * y2 +
                                              (10 * nx * r17 * w * x2 +
                                               nz * (10 * r17 * w2 +
                                                     r15) * x) * y -
                                              2 * ny * r15 * w * x) * zw -
        x * (ny * (10 * r17 * w2 + r15 - r05) * y2 - nz * r05 * y * z) +
        nx * (2 * r15 * w2 - (10 * r17 * w2 + r15 - r05) * x2) * y +
        2 * ny * r15 * w2 * x;
    t[XZ] =
        (x *
         (10 * r17 * w * (nz * zw + ny * y) + nz * (10 * r17 * w2 - r15)) +
         2 * nx * r15 * w) * zw2 + zw * (2 * nx * w * (5 * r17 * x2 -
                                                       r15) * zw +
                                         (10 * r17 * w2 -
                                          r15) * (ny * x * y + nx * x2) -
                                         2 * r15 * (2 * nz * w * x +
                                                    nx * w2)) +
        r05 * (nz * x * z2 + (ny * x * y + nx * x2) * z) -
        2 * r15 * (w * (ny * x * y + nx * x2) + nz * w2 * x);
    t[YX] =
        (-10 * nz * r17 * w * x * y * zw2) - (10 * ny * r17 * w * x * y2 +
                                              (10 * nx * r17 * w * x2 +
                                               nz * (10 * r17 * w2 +
                                                     r15) * x -
                                               2 * nx * r15 * w) * y) *
        zw - x * (ny * (10 * r17 * w2 + r15 - r05) * y2 -
                  nz * r05 * y * z) + nx * (2 * r15 * w2 - (10 * r17 * w2 +
                                                            r15 -
                                                            r05) * x2) *
        y + 2 * ny * r15 * w2 * x;
    t[YY] =
        zw * (2 * nz * w * (r15 - 5 * r17 * y2) * zw -
              10 * ny * r17 * w * y3 - (10 * nx * r17 * w * x +
                                        nz * (10 * r17 * w2 + r15)) * y2 +
              2 * r15 * (w * (2 * ny * y + nx * x) + nz * w2)) +
        nz * r05 * y2 * z - (10 * r17 * w2 + r15 - r05) * (ny * y3 +
                                                           nx * x * y2) +
        2 * r15 * w2 * (3 * ny * y + nx * x);
    t[YZ] =
        (y *
         (10 * r17 * w * (nz * zw + nx * x) + nz * (10 * r17 * w2 - r15)) +
         2 * ny * r15 * w) * zw2 +
        zw * (ny *
              (2 * w * (5 * r17 * y2 - r15) * zw +
               (10 * r17 * w2 - r15) * y2) + (nx * (10 * r17 * w2 -
                                                    r15) * x -
                                              4 * nz * r15 * w) * y -
              2 * ny * r15 * w2) + r05 * (nz * y * z2 + (ny * y2 +
                                                         nx * x * y) * z) -
        2 * r15 * (ny * w * y2 + (nx * w * x + nz * w2) * y);
    t[ZX] =
        (-x *
         (10 * r17 * w * (nz * zw + ny * y) +
          nz * (10 * r17 * w2 + r15)) * zw2) - zw * ((-2 * nx * w * (r15 -
                                                                     5 *
                                                                     r17 *
                                                                     x2) *
                                                      zw) +
                                                     ny * (10 * r17 * w2 +
                                                           r15) * x * y +
                                                     nx *
                                                     ((10 * r17 * w2 +
                                                       r15) * x2 -
                                                      2 * r15 * w2)) +
        r05 * (nz * x * z2 + (ny * x * y + nx * x2) * z) +
        2 * nz * r15 * w2 * x;
    t[ZY] =
        (-y *
         (10 * r17 * w * (nz * zw + nx * x) +
          nz * (10 * r17 * w2 + r15)) * zw2) - zw * ((-2 * ny * w * (r15 -
                                                                     5 *
                                                                     r17 *
                                                                     y2) *
                                                      zw) +
                                                     (10 * r17 * w2 +
                                                      r15) * (ny * y2 +
                                                              nx * x * y) -
                                                     2 * ny * r15 * w2) +
        r05 * (nz * y * z2 + (ny * y2 + nx * x * y) * z) +
        2 * nz * r15 * w2 * y;
    t[ZZ] =
        (10 * r17 * (w * zw + w2) - r15) * (nz * zw3 +
                                            (ny * y + nx * x) * zw2) -
        2 * r15 * w * (3 * nz * z +
                       2 * (ny * y + nx * x - 3 * nz * w)) * zw +
        r05 * (nz * z3 + (ny * y + nx * x) * z2) -
        2 * r15 * w2 * (3 * nz * z + ny * y + nx * x - 6 * nz * w);
    return CO_OK;
}

static real
F(real q, void *v)
{
    real s;
    real z;
    real z0;
    real W;
    struct Param *p;

    real(*f) (real, real, real, real);
    real(*j) (real);

    p = v;
    j = p->j;
    s = p->s;
    z = p->z;
    z0 = p->z0;
    W = p->W;
    f = p->f;
    return j(q * s) * f(q, z, z0, W);
}

static real
F0(real q, void *v)
{
    real s;
    real z;
    real z0;
    real W;
    real ans;
    struct Param0 *p;

    real(*f) (real, real, real);

    p = v;
    z = p->z;
    W = p->W;
    f = p->f;

    return f(q, z, W);
}

static real
fAp(real u)
{
    return u - exp(-2 * u) / 2 + 0.5;
}

static real
fAm(real u)
{
    return u + exp(-2 * u) / 2 - 0.5;
}

static real
fBp(real u)
{
    return u + exp(-2 * u) / 2 + 0.5;
}

static real
fBm(real u)
{
    return u - exp(-2 * u) / 2 - 0.5;
}

static real
fCp(real u)
{
    return sq(u) + u - exp(-2 * u) / 2 + 0.5;
}

static real
fCm(real u)
{
    return sq(u) + u + exp(-2 * u) / 2 - 0.5;
}

static real
fDp(real u)
{
    return sq(u) + u + exp(-2 * u) / 2 + 0.5;
}

static real
fDm(real u)
{
    return sq(u) + u - exp(-2 * u) / 2 - 0.5;
}

static real
fEp(real u)
{
    return 1 / (exp(-2 * u) / 4 + u - exp(-2 * u) / 4);
}

static real
fEm(real u)
{
    return 1 / (exp(2 * u) / 4 - u - exp(-2 * u) / 4);
}

static real
tnn(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Ap;
    real Bp;
    real Cp;
    real Dp;
    real Ep;
    real Em;

    u = W * q;
    v = q * z0;
    w = q * z;
    Ap = fAp(u);
    Bp = fBp(u);
    Cp = fCp(u);
    Dp = fDp(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Ep * (Ap * cosh(v) - v * sinh(v)) * w * sinh(w) +
        Em * (Bp * v * cosh(v) - Dp * sinh(v)) * sinh(w) +
        Em * (Bp * sinh(v) - v * cosh(v)) * w * cosh(w) +
        Ep * (Ap * v * sinh(v) - Cp * cosh(v)) * cosh(w);
}

static real
tnp(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Ap;
    real Am;
    real Bp;
    real Bm;
    real Ep;
    real Em;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    Ap = fAp(u);
    Am = fAm(u);
    Bp = fBp(u);
    Bm = fBm(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Ep * (v * cosh(v) - Bm * sinh(v)) * w * sinh(w) +
        Em * (u * u * cosh(v) - Bp * v * sinh(v)) * sinh(w) +
        Em * (v * sinh(v) - Am * cosh(v)) * w * cosh(w) +
        Ep * (u * u * sinh(v) - Ap * v * cosh(v)) * cosh(w);
}

static real
tpn(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Ap;
    real Am;
    real Bp;
    real Bm;
    real Ep;
    real Em;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    Ap = fAp(u);
    Am = fAm(u);
    Bp = fBp(u);
    Bm = fBm(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Em * (Bp * sinh(v) - v * cosh(v)) * w * sinh(w) +
        Ep * (Bm * v * sinh(v) - u * u * cosh(v)) * sinh(w) +
        Ep * (Ap * cosh(v) - v * sinh(v)) * w * cosh(w) +
        Em * (Am * v * cosh(v) - u * u * sinh(v)) * cosh(w);
}

static real
tpp(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;
    real Am;
    real Bm;
    real Cm;
    real Dm;
    real Ep;
    real Em;

    u = W * q;
    v = q * z0;
    w = q * z;
    Am = fAm(u);
    Bm = fBm(u);
    Cm = fCm(u);
    Dm = fDm(u);
    Ep = fEp(u);
    Em = fEm(u);

    if (u == 0)
        ERR(CO_NUM, "u == 0");
    return Em * (v * sinh(v) - Am * cosh(v)) * w * sinh(w) +
        Ep * ((-2 * xcoth(u) * sinh(v)) + Dm * sinh(v) -
              Bm * v * cosh(v)) * sinh(w) + Ep * (v * cosh(v) -
                                                  Bm * sinh(v)) * w *
        cosh(w) + Em * ((-Am * v * sinh(v)) - 2 * u * tanh(u) * cosh(v) +
                        Cm * cosh(v)) * cosh(w);
}

static real
rpp(real q, real z, real z0, real W)
{
    real u;
    real v;
    real w;

    u = W * q;
    u = W * q;
    v = q * z0;
    w = q * z;
    return (-(2 * exp(-u) * sinh(v) * sinh(w)) / sinh(u)) -
        (2 * exp(-u) * cosh(v) * cosh(w)) / cosh(u);
}

static int
f_xx_yy(struct Input *i, AlgIntegration * integration, real * xx,
        real * yy)
{
    real a;
    real b;
    real c;
    real s2;
    real x2;
    real y2;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.f = tpp;
    p.j = alg_special_bessel_J2;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &b) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    p.j = alg_special_bessel_J0;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &a) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    p.f = rpp;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &c) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *xx = -0.5 * a - 0.5 * (y2 - x2) / s2 * b + c;
    *yy = -0.5 * a - 0.5 * (x2 - y2) / s2 * b + c;
    return CO_OK;
}

static int
f_zz(struct Input *i, AlgIntegration * integration, real * result)
{
    real s2;
    real x2;
    real y2;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J0;
    p.f = tnn;
    if (alg_integration_apply(integration, 0, qmax, F, &p, result) !=
        CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    return CO_OK;
}

static int
f_xy(struct Input *i, AlgIntegration * integration, real * result)
{
    real s2;
    real x2;
    real y2;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J2;
    p.f = tpp;
    if (alg_integration_apply(integration, 0, qmax, F, &p, result) !=
        CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");

    *result *= (i->x * i->y) / s2;
    return CO_OK;
}

static int
f_xz_yz(struct Input *i, AlgIntegration * integration, real * xz,
        real * yz)
{
    real s2;
    real x2;
    real y2;
    real s;
    real result;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J1;
    p.f = tpn;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &result) !=
        CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *xz = -(i->x) * result / s;
    *yz = -(i->y) * result / s;
    return CO_OK;
}

static int
f_zx_zy(struct Input *i, AlgIntegration * integration, real * zx,
        real * zy)
{
    real s2;
    real x2;
    real y2;
    real s;
    real result;
    struct Param p;

    x2 = i->x * i->x;
    y2 = i->y * i->y;
    s2 = x2 + y2;
    if (s2 == 0)
        ERR(CO_NUM, "s2 == 0");
    p.s = s = sqrt(s2);
    p.z = i->z;
    p.z0 = i->z0;
    p.W = i->W;
    p.j = alg_special_bessel_J1;
    p.f = tnp;
    if (alg_integration_apply(integration, 0, qmax, F, &p, &result) !=
        CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *zx = -(i->x) * result / s;
    *zy = -(i->y) * result / s;
    return CO_OK;
}

static int
f_xx_yy0(struct Input0 *i, AlgIntegration * integration, real * xx,
         real * yy)
{
    real a;
    real b;
    real s2;
    real x2;
    real y2;
    struct Param0 p;

    p.z = i->z;
    p.W = i->W;
    p.f = tpp0;
    if (alg_integration_apply(integration, 0, qmax, F0, &p, &a) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    p.f = rpp0;
    if (alg_integration_apply(integration, 0, qmax, F0, &p, &b) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    *yy = *xx = -a / 2 + b;
    return CO_OK;
}

static int
f_zz0(struct Input0 *i, AlgIntegration * integration, real * zz)
{
    struct Param0 p;

    p.z = i->z;
    p.W = i->W;
    p.f = tnn0;
    if (alg_integration_apply(integration, 0, qmax, F0, &p, zz) != CO_OK)
        ERR(CO_NUM, "alg_integration_apply failed");
    return CO_OK;
}

static real
tnn0(real q, real z, real W)
{
    real u;
    real v;
    real Ap;
    real Bp;
    real Cp;
    real Dp;
    real Ep;
    real Em;

    u = W * q;
    v = q * z;
    Ap = fAp(u);
    Bp = fBp(u);
    Cp = fCp(u);
    Dp = fDp(u);
    Ep = fEp(u);
    Em = fEm(u);
    return Ep * cosh(v) * (Ap * v * sinh(v) - Cp * cosh(v)) +
        Ep * v * sinh(v) * (Ap * cosh(v) - v * sinh(v)) +
        Em * sinh(v) * (Bp * v * cosh(v) - Dp * sinh(v)) +
        Em * v * cosh(v) * (Bp * sinh(v) - v * cosh(v));
}

static real
rpp0(real q, real z, real W)
{
    real u;
    real v;

    u = W * q;
    v = q * z;
    return (-(2 * exp(-u) * sq(sinh(v))) / sinh(u)) -
        (2 * exp(-u) * sq(cosh(v))) / cosh(u);
}

static real
tpp0(real q, real z, real W)
{
    real u;
    real v;
    real Am;
    real Bm;
    real Cm;
    real Dm;
    real Ep;
    real Em;

    u = W * q;
    v = q * z;
    Am = fAm(u);
    Bm = fBm(u);
    Cm = fCm(u);
    Dm = fDm(u);
    Ep = fEp(u);
    Em = fEm(u);

    return Em * cosh(v) * ((-Am * v * sinh(v)) -
                           2 * u * tanh(u) * cosh(v) + Cm * cosh(v)) +
        Em * v * sinh(v) * (v * sinh(v) - Am * cosh(v)) +
        Ep * sinh(v) * ((-2 * xcoth(u) * sinh(v)) + Dm * sinh(v) -
                        Bm * v * cosh(v)) +
        Ep * v * cosh(v) * (v * cosh(v) - Bm * sinh(v));
}

static real
sq(real x)
{
    return x * x;
}

static
    real
xcoth(real x)
{                               /* x*coth(x) */
    return fabs(x) > 0.25 ? x / tanh(x) : 1 + sq(x) / 3;
}
