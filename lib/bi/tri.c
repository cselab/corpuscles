#include <stdio.h>
#include "real.h"
#include "co/argv.h"
#include "co/bi/tri.h"
#include "co/err.h"
#include "co/he.h"
#include "co/i/matrix.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/oseen3tri.h"
#include "co/tensor.h"

#define T BiTri
static const real pi = 3.141592653589793115997964;
struct T {
    Oseen3Tri *oseen;
    struct Tensor O, K;
    int KReady;
};

int
bi_tri_ini(He *he, /**/ T **pq)
{
    T *q;
    int status, n;

    MALLOC(1, &q);
    status = oseen3_tri_ini(he, &q->oseen);
    if (status != CO_OK)
	ERR(CO_MEMORY, "oseen3_tri_ini failed");
    n = he_nv(he);
    tensor_ini(n, &q->O);
    tensor_ini(n, &q->K);
    q->KReady = 0;
    *pq = q;
    return CO_OK;
}

int
bi_tri_argv(char ***p, He *he, /**/ T **pq)
{
    return bi_tri_ini(he, pq);
}

int
bi_tri_fin(T *q)
{
    oseen3_tri_fin(q->oseen);
    tensor_fin(&q->O);
    tensor_fin(&q->K);
    FREE(q);
    return CO_OK;
}

int
bi_tri_update(T *q, He *he, const real *x, const real *y, const real *z)
{
    struct Tensor *O, *K;
    int status, n, i;
    real coeff;

    O = &q->O;
    K = &q->K;
    status = oseen3_tri_apply(q->oseen, he, x, y, z, O->xx, O->xy, O->xz, O->yy, O->yz, O->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_tri_apply failed");
    q->KReady = 0;
    return CO_OK;
}

int
bi_tri_single(T *q, He *he, real al,
		 const real *x, const real *y, const real *z,
		 const real *fx, const real *fy, const real *fz,
		 /*io*/ real *ux, real *uy, real *uz)
{
    struct Tensor *O;
    int n, status;
    USED(x);
    USED(y);
    USED(z);

    O = &q->O;
    n = he_nv(he);
    tensor_vector(n, al, fx, fy, fz, O, ux, uy, uz);
    return CO_OK;
}

int
bi_tri_double(T *q, He *he, real al,
		 const real *x, const real *y, const real *z,
		 const real *ux, const real *uy, const real *uz,
		 /*io*/real *wx, real *wy, real *wz)
{
    int n, status;
    struct Tensor *K;
    USED(x);
    USED(y);
    USED(z);

    K = &q->K;
    if (q->KReady == 0) {
	status = oseen3_tri_stresslet(q->oseen, he, x, y, z, K->xx, K->xy, K->xz, K->yy, K->yz, K->zz);
	if (status != CO_OK)
	    ERR(CO_NUM, "oseen3_tri_stresslet failed");
	q->KReady = 1;
    }
    n = he_nv(he);
    tensor_vector(n, al, ux, uy, uz, K, wx, wy, wz);
    return CO_OK;
}
