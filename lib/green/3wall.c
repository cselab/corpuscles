#include <math.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/green/3wall.h"
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

#define T Green3Wall
enum { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };
static const real pi = 3.141592653589793115997964;
struct T {
    real w;
    real *nx, *ny, *nz, *area;
};


int
green3_wall_ini(He * he, real w, T ** pq)
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
    *pq = q;
    return CO_OK;
}

int
green3_wall_fin(T * q)
{
    FREE3(q->nx, q->ny, q->nz);
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

int
green3_wall_apply(T * q, He * he, const real * x, const real * y,
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
                green3_wall_s0(q, a, &t0);
            else {
                i_vec_get(j, x, y, z, b);
                if (green3_wall_s(q, a, b, &t0) != CO_OK)
                    ERR(CO_NUM, "green3_wall_s failed (i=%d, j=%d)", i, j);
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
green3_wall_stresslet(T * q, He * he, const real * x, const real * y,
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
                green3_wall_t0(q, a, u, &t0);
            } else {
                i_vec_get(j, x, y, z, b);
                if (green3_wall_t(q, a, u, b, &t0) != CO_OK)
                    ERR(CO_NUM, "green3_wall_s failed (i=%d, j=%d)", i, j);
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
green3_wall_s0(T * q, const real a[3], Ten * t0)
{
    enum {
        X, Y, Z
    };
    real w;
    real *t;
    real r1;
    real r13;
    real r15;
    real w2;
    real zw;

    t = t0->t;
    w = q->w - a[Z];
    zw = -2 * w;
    r1 = 1 / fabs(zw);
    r13 = r1 * r1 * r1;
    r15 = r1 * r1 * r1 * r1 * r1;
    w2 = w * w;
    t[XX] = (-2 * r13 * w2) - r1;
    t[YY] = t[XX];
    t[ZZ] = (-2 * w2 * (12 * r15 * w2 - r13)) - 4 * r13 * w2 - r1;
    t[XY] = 0;
    t[XZ] = 0;
    t[ZX] = 0;
    t[YX] = 0;
    t[YZ] = 0;
    t[ZY] = 0;
    return CO_OK;
}

int
green3_wall_t0(T * q, const real a[3], const real n[3], Ten * t0)
{
    enum {
        X, Y, Z
    };
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
green3_wall_s(T * q, const real a[3], const real b[3], Ten * t0)
{
    enum {
        X, Y, Z
    };
    real d[3];
    real w;
    real *t;
    real r0;
    real r1;
    real x;
    real y;
    real z;
    real r13;
    real r15;
    real r03;
    real w2;
    real x2;
    real y2;
    real z2;
    real zw;
    real zw2;

    t = t0->t;
    w = q->w - b[Z];
    i_vec_minus(a, b, d);

    x = d[X];
    y = d[Y];
    z = d[Z];
    zw = z - 2 * w;
    r0 = 1 / rad(x, y, z);
    r1 = 1 / rad(x, y, zw);
    r13 = r1 * r1 * r1;
    r15 = r1 * r1 * r1 * r1 * r1;
    r03 = r0 * r0 * r0;
    w2 = w * w;
    x2 = x * x;
    y2 = y * y;
    z2 = z * z;
    zw2 = zw * zw;

    t[XX] =
        2 * w * (r13 - 3 * r15 * x2) * zw + 2 * w2 * (r13 - 3 * r15 * x2) -
        r13 * x2 + r03 * x2 - r1 + r0;
    t[YY] =
        2 * w * (r13 - 3 * r15 * y2) * zw + 2 * w2 * (r13 - 3 * r15 * y2) -
        r13 * y2 + r03 * y2 - r1 + r0;
    t[ZZ] =
        2 * w * zw * (3 * r15 * zw2 - r13) + 2 * w2 * (3 * r15 * zw2 -
                                                       r13) - r13 * zw2 +
        r03 * z2 - r1 + r0;
    t[XY] =
        (-6 * r15 * w * x * y * zw) - 6 * r15 * w2 * x * y - r13 * x * y +
        r03 * x * y;
    t[XZ] =
        2 * w * (3 * r15 * x * zw2 - r13 * x) + 6 * r15 * w2 * x * zw -
        r13 * x * zw + r03 * x * z;
    t[ZX] =
        2 * w * ((-3 * r15 * x * zw2) - r13 * x) - 6 * r15 * w2 * x * zw -
        r13 * x * zw + r03 * x * z;
    t[YX] = t[XY];
    t[YZ] =
        2 * w * (3 * r15 * y * zw2 - r13 * y) + 6 * r15 * w2 * y * zw -
        r13 * y * zw + r03 * y * z;
    t[ZY] =
        2 * w * ((-3 * r15 * y * zw2) - r13 * y) - 6 * r15 * w2 * y * zw -
        r13 * y * zw + r03 * y * z;
    return CO_OK;
}

int
green3_wall_t(T * q, const real a[3], const real n[3], const real b[3],
              Ten * t0)
{
    enum {
        X, Y, Z
    };
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
