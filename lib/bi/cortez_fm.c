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
#include "co/oseen3zero.h"
#include "co/tensor.h"
#include "co/bi/cortez_fm.h"

#define T BiCortezFm
struct T {
    FM *fm;
    real *wx, *wy, *wz, *area;
    real *nx, *ny, *nz;
    Oseen3Zero *oseen;
    struct Tensor O, K;
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
    status = oseen3_zero_ini(he, eps, &q->oseen);
    if (status != CO_OK)
	ERR(CO_MEMORY, "oseen3_zero_ini failed");
    status = fm_ini(n, &q->fm);
    if (status != CO_OK)
	ERR(CO_MEMORY, "fm_ini failed");	
    MALLOC3(n, &q->wx, &q->wy, &q->wz);
    MALLOC3(n, &q->nx, &q->ny, &q->nz);
    MALLOC(n, &q->area);
    tensor_ini(n, &q->O);
    tensor_ini(n, &q->K);
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
    oseen3_zero_fin(q->oseen);
    tensor_fin(&q->O);
    tensor_fin(&q->K);
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
    struct Tensor *O, *K;
    real *area, *nx, *ny, *nz;
    int status;

    O = &q->O;
    K = &q->K;
    area = q->area;
    nx = q->nx;
    ny = q->ny;
    nz = q->nz;
    
    status = oseen3_zero_apply(q->oseen, he, x, y, z, O->xx, O->xy, O->xz, O->yy, O->yz, O->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_zero_apply failed");
    status = oseen3_zero_stresslet(q->oseen, he, x, y, z, K->xx, K->xy, K->xz, K->yy, K->yz, K->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_zero_stresslet failed");
    he_area_ver(he, x, y, z, area);
    normal_mwa(he, x, y, z, nx, ny, nz);
    return CO_OK;
}

int
bi_cortez_fm_single(T *q, He *he, real al, const real *x, const real *y, const real *z, const real *fx, const real *fy, const real *fz, /*io*/ real *ux, real *uy, real *uz)
{
    real *wx, *wy, *wz;
    int n;

    wx = q->wx;
    wy = q->wy;
    wz = q->wz;
    n = he_nv(he);
    array_zero3(n, wx, wy, wz);
    fm_single(q->fm, x, y, z, fx, fy, fz, wx, wy, wz);
    array_axpy3(n, al, wx, wy, wz, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_fm_double(T *q, He *he, real al, const real *x, const real *y, const real *z, const real *ux, const real *uy, const real *uz, /*io*/real *wx, real *wy, real *wz)
{
    int n;
    struct Tensor *K;
    USED(x);
    USED(y);
    USED(z);
    
    K = &q->K;
    n = he_nv(he);
    tensor_vector(n, al, ux, uy, uz, K, wx, wy, wz);
    return CO_OK;
}
