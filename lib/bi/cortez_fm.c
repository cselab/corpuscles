#include <stdio.h>
#include <math.h>
#include "real.h"
#include "fm/fm.h"
#include "co/area.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/argv.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/vec.h"
#include "co/bi/cortez_fm.h"
#include "co/bi/self_circle.h"

#define T BiCortezFm
struct T {
    BiSelfCircle *self;
    FM *fm;
    real *wx, *wy, *wz, *area;
    real *nx, *ny, *nz;
    real *ax, *ay, *az;
};

int
bi_cortez_fm_ini(He * he, /**/ T ** pq)
{
    T *q;
    int status, n;

    n = he_nv(he);
    MALLOC(1, &q);
    status = fm_ini(n, &q->fm);
    if (status != CO_OK)
        ERR(CO_MEMORY, "fm_ini failed");
    status = bi_self_circle_ini(he, &q->self);
    if (status != CO_OK)
        ERR(CO_MEMORY, "bi_self_circle_ini failed");
    MALLOC3(n, &q->wx, &q->wy, &q->wz);
    MALLOC3(n, &q->nx, &q->ny, &q->nz);
    MALLOC3(n, &q->ax, &q->ay, &q->az);
    MALLOC(n, &q->area);
    *pq = q;
    return CO_OK;
}

int
bi_cortez_fm_argv(char ***p, He * he, /**/ T ** pq)
{
    USED(p);
    return bi_cortez_fm_ini(he, pq);
}

int
bi_cortez_fm_fin(T * q)
{
    fm_fin(q->fm);
    bi_self_circle_fin(q->self);
    FREE3(q->wx, q->wy, q->wz);
    FREE3(q->nx, q->ny, q->nz);
    FREE3(q->ax, q->ay, q->az);
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

int
bi_cortez_fm_update(T * q, He * he, const real * x, const real * y,
                    const real * z)
{
    real *area, *nx, *ny, *nz;
    int status;

    area = q->area;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    status = bi_self_circle_update(q->self, he, x, y, z);
    status = he_area_ver(he, x, y, z, area);
    if (status != CO_OK)
        ERR(CO_NUM, "he_area_ver failed");
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
        ERR(CO_NUM, "normal_mwa failed");
    return CO_OK;
}

int
bi_cortez_fm_single(T * q, He * he, real al, const real * x,
                    const real * y, const real * z, const real * fx,
                    const real * fy, const real * fz, /*io */ real * ux,
                    real * uy, real * uz)
{
    real *wx, *wy, *wz;
    int n, status;

    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    status = fm_single(q->fm, x, y, z, fx, fy, fz, wx, wy, wz);
    if (status != CO_OK)
        ERR(CO_NUM, "fm_single failed");
    status =
        bi_self_circle_single(q->self, he, al, x, y, z, fx, fy, fz, ux, uy,
                              uz);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_single failed");
    array_axpy3(n, al, wx, wy, wz, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_fm_double(T * q, He * he, real alpha, const real * x,
                    const real * y, const real * z, const real * ux,
                    const real * uy, const real * uz, /*io */ real * vx,
                    real * vy, real * vz)
{
    int n, status;
    real *wx, *wy, *wz, *ax, *ay, *az;
    const real *nx, *ny, *nz, *area;

    USED(x);
    USED(y);
    USED(z);
    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    ax = q->ax;
    ay = q->ay;
    az = q->az;
    area = q->area;
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    array_copy3(n, nx, ny, nz, ax, ay, az);
    array_multiply3(n, area, ax, ay, az);
    status = fm_double(q->fm, x, y, z, ux, uy, uz, ax, ay, az, wx, wy, wz);
    if (status != CO_OK)
        ERR(CO_NUM, "fm_double failed");
    bi_self_circle_double(q->self, he, alpha, x, y, z, ux, uy, uz, vx, vy,
                          vz);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_double failed");
    array_axpy3(n, alpha, wx, wy, wz, vx, vy, vz);
    return CO_OK;
}
