#include <stdio.h>
#include <math.h>
#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/argv.h"
#include "co/curv/mean/juelicher.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/vec.h"
#include "co/bi/self_circle.h"

static const real pi = 3.141592653589793115997964;

#define T BiSelfCircle
struct T {
    CurvMeanJuelicher *H;
    real *wx, *wy, *wz, *area;
    real *nx, *ny, *nz;
    real *h;
};

int
bi_self_circle_ini(He * he, /**/ T ** pq)
{
    T *q;
    int status, n;

    n = he_nv(he);
    MALLOC(1, &q);
    status = curv_mean_juelicher_ini(he, &q->H);
    if (status != CO_OK)
        ERR(CO_MEMORY, "H_ini failed");
    MALLOC3(n, &q->wx, &q->wy, &q->wz);
    MALLOC3(n, &q->nx, &q->ny, &q->nz);
    MALLOC(n, &q->area);
    MALLOC(n, &q->h);
    *pq = q;
    return CO_OK;
}

int
bi_self_circle_argv(char ***p, He * he, /**/ T ** pq)
{
    USED(p);
    return bi_self_circle_ini(he, pq);
}

int
bi_self_circle_fin(T * q)
{
    curv_mean_juelicher_fin(q->H);
    FREE3(q->wx, q->wy, q->wz);
    FREE3(q->nx, q->ny, q->nz);
    FREE(q->area);
    FREE(q->h);
    FREE(q);
    return CO_OK;
}

int
bi_self_circle_update(T * q, He * he, const real * x, const real * y,
                      const real * z)
{
    real *area, *area0, *nx, *ny, *nz, *h;
    int n, status;

    area = q->area;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    h = q->h;
    status = curv_mean_juelicher_apply(q->H, he, x, y, z, h);
    if (status != CO_OK)
        ERR(CO_NUM, "curv_mean_juelicher_apply failed");
    n = he_nv(he);
    curv_mean_juelicher_area(q->H, &area0);
    array_copy(n, area0, area);
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
        ERR(CO_NUM, "normal_mwa failed");
    return CO_OK;
}

int
bi_self_circle_single(T * q, He * he, real al, const real * x,
                      const real * y, const real * z, const real * fx,
                      const real * fy, const real * fz, /*io */ real * ux,
                      real * uy, real * uz)
{
    real *wx, *wy, *wz, *nx, *ny, *nz, *area, normal[3], force[3],
        reject[3];
    real A, fX, fZ, wX, wZ;
    int n, i;

    USED(x);
    USED(y);
    USED(z);
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    area = q->area;
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    for (i = 0; i < n; i++) {
        A = area[i];
        vec_get(i, nx, ny, nz, normal);
        vec_get(i, fx, fy, fz, force);
        fX = vec_reject_scalar(force, normal);
        fZ = vec_project_scalar(force, normal);
        vec_reject(force, normal, reject);
        vec_normalize(reject);
        //wX = -(5*A*h[i]*h[i]-72*pi)/(24*sqrt(pi)*sqrt(A));
        //wZ = (A*h[i]*h[i]+24*pi)/(12*sqrt(pi)*sqrt(A));
        wX = 3 * sqrt(pi) / sqrt(A);
        wZ = 2 * sqrt(pi) / sqrt(A);
        vec_scalar_append(reject, fX * wX / (8 * pi), i, wx, wy, wz);
        vec_scalar_append(normal, fZ * wZ / (8 * pi), i, wx, wy, wz);
    }
    array_axpy3(n, al, wx, wy, wz, ux, uy, uz);
    return CO_OK;
}

int
bi_self_circle_double(T * q, He * he, real alpha, const real * x,
                      const real * y, const real * z, const real * ux,
                      const real * uy, const real * uz, /*io */ real * vx,
                      real * vy, real * vz)
{
    int i, n;
    real *wx, *wy, *wz, *h;
    const real *area;
    real normal[3], velocity[3], reject[3];
    const real *nx;
    const real *ny;
    const real *nz;
    real uX, p, A;

    USED(x);
    USED(y);
    USED(z);
    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    area = q->area;
    h = q->h;
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    for (i = 0; i < n; i++) {
        A = area[i];
        p = sqrt(A) / sqrt(pi) * h[i];
        vec_get(i, nx, ny, nz, normal);
        vec_get(i, ux, uy, uz, velocity);
        uX = vec_reject_scalar(velocity, normal);
        vec_reject(velocity, normal, reject);
        vec_normalize(reject);
        vec_scalar_append(reject, -3 * uX * p / 8, i, wx, wy, wz);
    }
    array_axpy3(n, alpha, wx, wy, wz, vx, vy, vz);
    return CO_OK;
}
