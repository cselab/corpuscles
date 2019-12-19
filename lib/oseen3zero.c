#include <math.h>
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/oseen3zero.h"
#include "co/i/matrix.h"
#include "co/i/vec.h"
#include "co/i/tri.h"
#include "co/i/area.h"

#define T Oseen3Zero
static const real pi = 3.141592653589793115997964;
struct T {
    real *nx, *ny, *nz, *area;
};

static int oseen(const real[3], const real[3], real *, real *, real *,
                 real *, real *, real *);
static int stresslet(const real[3], const real[3], const real[3], real *,
                     real *, real *, real *, real *, real *);

int
oseen3_zero_ini(He * he, T ** pq)
{
    T *q;
    real *nx, *ny, *nz, *area;
    int n;

    MALLOC(1, &q);
    n = he_nv(he);
    MALLOC3(n, &nx, &ny, &nz);
    MALLOC(n, &area);
    q->nx = nx;
    q->ny = ny;
    q->nz = nz;
    q->area = area;
    *pq = q;
    return CO_OK;
}

int
oseen3_zero_fin(T * q)
{
    FREE3(q->nx, q->ny, q->nz);
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

#define SET(i, j, s, a) i_matrix_set(n, n, i, j, s, a)
int
oseen3_zero_apply(T * q, He * he, const real * x, const real * y,
                  const real * z, real * oxx, real * oxy, real * oxz,
                  real * oyy, real * oyz, real * ozz)
{
    int n, i;
    real s;

    USED(q);

    n = he_nv(he);
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        real a[3], b[3], xx, xy, xz, yy, yz, zz;
        int j;

        i_vec_get(i, x, y, z, a);
        SET(i, i, 0, oxx);
        SET(i, i, 0, oxy);
        SET(i, i, 0, oxz);
        SET(i, i, 0, oyy);
        SET(i, i, 0, oyz);
        SET(i, i, 0, ozz);
        for (j = i + 1; j < n; j++) {
            i_vec_get(j, x, y, z, b);
            if (oseen(a, b, &xx, &xy, &xz, &yy, &yz, &zz) != CO_OK)
                ERR(CO_NUM, "ossen failed (i=%d, j=%d)", i, j);
            SET(i, j, xx, oxx);
            SET(i, j, xy, oxy);
            SET(i, j, xz, oxz);
            SET(i, j, yy, oyy);
            SET(i, j, yz, oyz);
            SET(i, j, zz, ozz);

            SET(j, i, xx, oxx);
            SET(j, i, xy, oxy);
            SET(j, i, xz, oxz);
            SET(j, i, yy, oyy);
            SET(j, i, yz, oyz);
            SET(j, i, zz, ozz);
        }
    }
    s = 1 / (8 * pi);
    i_matrix_scale(n, n, s, oxx);
    i_matrix_scale(n, n, s, oxy);
    i_matrix_scale(n, n, s, oxz);
    i_matrix_scale(n, n, s, oyy);
    i_matrix_scale(n, n, s, oyz);
    i_matrix_scale(n, n, s, ozz);
    return CO_OK;
}

int
oseen3zero_single_velocity(T * q, He * he,
                           const real * x, const real * y, const real * z,
                           const real * fx, const real * fy,
                           const real * fz, const real a[3],
                           /**/ real v[3])
{
    int n, i;
    real s, xx, xy, xz, yy, yz, zz, b[3], f[3];
    real dx, dy, dz;

    USED(q);
    USED(he);
    enum { X, Y, Z };

    n = he_nv(he);
    dx = dy = dz = 0;
    for (i = 0; i < n; i++) {
        i_vec_get(i, x, y, z, b);
        i_vec_get(i, fx, fy, fz, f);
        oseen(a, b, &xx, &xy, &xz, &yy, &yz, &zz);
        dx += xx * f[X] + xy * f[Y] + xz * f[Z];
        dy += xy * f[X] + yy * f[Y] + yz * f[Z];
        dz += xz * f[X] + yz * f[Y] + zz * f[Z];
    }
    s = 1 / (8 * pi);
    v[X] = s * dx;
    v[Y] = s * dy;
    v[Z] = s * dz;
    return CO_OK;
}

int
oseen3_zero_stresslet(T * q, He * he, const real * x, const real * y,
                      const real * z, real * oxx, real * oxy, real * oxz,
                      real * oyy, real * oyz, real * ozz)
{
    real *nx, *ny, *nz, *area, A;
    int status, n, i;

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
#pragma omp parallel for
    for (i = 0; i < n; i++) {
        int j;
        real a[3], b[3], u[3], xx, xy, xz, yy, yz, zz;

        i_vec_get(i, x, y, z, a);
        SET(i, i, 0, oxx);
        SET(i, i, 0, oxy);
        SET(i, i, 0, oxz);
        SET(i, i, 0, oyy);
        SET(i, i, 0, oyz);
        SET(i, i, 0, ozz);
        for (j = 0; j < n; j++) {
            if (i == j)
                continue;
            i_vec_get(j, nx, ny, nz, u);
            i_vec_get(j, x, y, z, b);
            A = 3 * area[j] / (4 * pi);
            stresslet(a, u, b, &xx, &xy, &xz, &yy, &yz, &zz);
            SET(i, j, A * xx, oxx);
            SET(i, j, A * xy, oxy);
            SET(i, j, A * xz, oxz);
            SET(i, j, A * yy, oyy);
            SET(i, j, A * yz, oyz);
            SET(i, j, A * zz, ozz);
        }
    }
    return CO_OK;
}

int
oseen3zero_double_velocity(T * q, He * he,
                           const real * x, const real * y, const real * z,
                           const real * ux, const real * uy,
                           const real * uz, const real r[3],
                           /**/ real v[3])
{
    enum { X, Y, Z };
    real *nx, *ny, *nz, *area, A, s;
    int n, i, status;
    real xx, xy, xz, yy, yz, zz, b[3], u[3], normal[3];
    real dx, dy, dz;

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
    dx = dy = dz = 0;
    for (i = 0; i < n; i++) {
        i_vec_get(i, nx, ny, nz, normal);
        i_vec_get(i, x, y, z, b);
        i_vec_get(i, ux, uy, uz, u);
        A = area[i];
        stresslet(r, normal, b, &xx, &xy, &xz, &yy, &yz, &zz);
        dx += A * (xx * u[X] + xy * u[Y] + xz * u[Z]);
        dy += A * (xy * u[X] + yy * u[Y] + yz * u[Z]);
        dz += A * (xz * u[X] + yz * u[Y] + zz * u[Z]);
    }
    s = 3 / (4 * pi);
    v[X] = s * dx;
    v[Y] = s * dy;
    v[Z] = s * dz;
    return CO_OK;
}

static int
oseen(const real a[3], const real b[3],
      real * xx, real * xy, real * xz, real * yy, real * yz, real * zz)
{
    enum {
        X, Y, Z
    };
    real d[3], r, r3, l;

    i_vec_minus(a, b, d);
    r = i_vec_abs(d);
    r3 = r * r * r;
    l = 1 / r;
    if (r == 0)
        ERR(CO_NUM, "r == 0");
    *xx = l + d[X] * d[X] / r3;
    *yy = l + d[Y] * d[Y] / r3;
    *zz = l + d[Z] * d[Z] / r3;
    *xy = d[X] * d[Y] / r3;
    *xz = d[X] * d[Z] / r3;
    *yz = d[Y] * d[Z] / r3;
    return CO_OK;
}

static int
stresslet(const real a[3], const real n[3], const real b[3],
          real * xx, real * xy, real * xz, real * yy, real * yz, real * zz)
{
    enum {
        X, Y, Z
    };
    real d[3], r, p, l;

    i_vec_minus(a, b, d);
    r = i_vec_abs(d);
    p = i_vec_dot(d, n);
    if (r == 0)
        ERR(CO_NUM, "r == 0");
    l = p / (r * r * r * r * r);
    *xx = d[X] * d[X] * l;
    *xy = d[X] * d[Y] * l;
    *xz = d[X] * d[Z] * l;
    *yy = d[Y] * d[Y] * l;
    *yz = d[Y] * d[Z] * l;
    *zz = d[Z] * d[Z] * l;
    return CO_OK;
}
