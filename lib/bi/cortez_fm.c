#include <stdio.h>
#include <tgmath.h>
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
#include "co/vec.h"
#include "co/bi/cortez_fm.h"

static const real pi = 3.141592653589793115997964;

#define T BiCortezFm
struct T {
    FM *fm;
    real *wx, *wy, *wz, *area;
    real *nx, *ny, *nz;
    real *ax, *ay, *az;
    real eps;
};

int
bi_cortez_fm_ini(real eps, He *he, /**/ T **pq)
{
    T *q;
    int status, n;

    n = he_nv(he);
    MALLOC(1, &q);
    if (eps <= 0)
	ERR(CO_IO, "eps=%g <= 0", eps);
    q->eps = eps;
    status = fm_ini(n, &q->fm);
    if (status != CO_OK)
	ERR(CO_MEMORY, "fm_ini failed");
    MALLOC3(n, &q->wx, &q->wy, &q->wz);
    MALLOC3(n, &q->nx, &q->ny, &q->nz);
    MALLOC3(n, &q->ax, &q->ay, &q->az);
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
    FREE3(q->ax, q->ay, q->az);
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
    real *wx, *wy, *wz, *nx, *ny, *nz, *area, normal[3], force[3], reject[3];
    real A, R, fX, fZ;
    int n, i, status;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    area = q->area;
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    status = fm_single(q->fm, x, y, z, fx, fy, fz, wx, wy, wz);
    
    //array_axpy3(n, 1/(4*pi*eps), fx, fy, fz, wx, wy, wz); /* self */
    for (i = 0; i < n; i++) {
	A = area[i];
	R = sqrt(A/pi);
	vec_get(i, nx, ny, nz, normal);
	vec_get(i, fx, fy, fz, force);
	fX = vec_reject_scalar(force, normal);
	fZ = vec_project_scalar(force, normal);
	vec_reject(force, normal, reject);
	vec_normalize(reject);
	vec_scalar_append(reject, 2*fX/(4*pi*R), i, wx, wy, wz);
	vec_scalar_append(normal, 3*fZ/(4*pi*R), i, wx, wy, wz);
    }
    if (status != CO_OK)
	ERR(CO_NUM, "fm_single failed");
    array_axpy3(n, al, wx, wy, wz, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_fm_double(T *q, He *he, real alpha, const real *x, const real *y, const real *z, const real *ux, const real *uy, const real *uz, /*io*/ real *vx, real *vy, real *vz)
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
    array_axpy3(n, alpha, wx, wy, wz, vx, vy, vz);
    return CO_OK;
}
