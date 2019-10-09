#include <stdio.h>
#include "real.h"
#include <fm.h>
#include "co/area.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/argv.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/normal.h"
#include "co/bi/cortez_fm.h"

#define T BiCortezFm
struct T {
    FM *fm;
    real *wx, *wy, *wz, *area;
    real *nx, *ny, *nz;
};

int
bi_cortez_fm_ini(real eps, He *he, /**/ T **pq)
{
    T *q;
    int status, n;

    MALLOC(1, &q);
    if (eps <= 0)
	ERR(CO_IO, "eps=%g <= 0", eps);
    n = he_nv(he);
    status = fm_ini(n, &q->fm);
    if (status != CO_OK)
	ERR(CO_MEMORY, "fm_ini failed");
    MALLOC3(n, &q->wx, &q->wy, &q->wz);
    MALLOC3(n, &q->nx, &q->ny, &q->nz);
    MALLOC(n, &q->area);
    *pq = q;
    return CO_OK;
}

int
bi_cortez_fm_argv(char ***p, He *he, /**/ T **pq)
{
    int status;
    real x;
    if ((status = argv_real(p, &x)) != CO_OK)
	return status;
    return bi_cortez_fm_ini(x, he, pq);
}

int
bi_cortez_fm_fin(T *q)
{
    fm_fin(q->fm);
    FREE3(q->wx, q->wy, q->wz);
    FREE3(q->nx, q->ny, q->nz);
    FREE(q->area);
    FREE(q);
    return CO_OK;
}

int
bi_cortez_fm_update(T *q, He *he, const real *x, const real *y, const real *z)
{
    real *area, *nx, *ny, *nz;
    int status;

    area = q->area;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    status = he_area_ver(he, x, y, z, area);
    if (status != CO_OK)
	ERR(CO_NUM, "he_area_ver failed");
    status = normal_mwa(he, x, y, z, nx, ny, nz);
    if (status != CO_OK)
	ERR(CO_NUM, "normal_mwa failed");
    return CO_OK;
}

int
bi_cortez_fm_single(T *q, He *he, real al, const real *x, const real *y, const real *z, const real *fx, const real *fy, const real *fz, /*io*/ real *ux, real *uy, real *uz)
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
    array_axpy3(n, al, wx, wy, wz, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_fm_double(T *q, He *he, real alpha, const real *x, const real *y, const real *z, const real *ux, const real *uy, const real *uz, /*io*/ real *vx, real *vy, real *vz)
{
    int n, status;
    real *wx, *wy, *wz, *nx, *ny, *nz, *area;
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
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    
    status = fm_double(q->fm, x, y, z, ux, uy, uz, nx, ny, nz, wx, wy, wz);
    if (status != CO_OK)
	ERR(CO_NUM, "fm_double failed");
    array_multiply3(n, area, wx, wy, wz);
    array_axpy3(n, alpha, wx, wy, wz, vx, vy, vz);
    return CO_OK;
}
