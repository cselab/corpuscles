#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/matrix.h"
#include "co/memory.h"
#include "co/oseen3.h"
#include "co/bi/cortez.h"

#define T BiCortez

struct Tensor {
    real *xx, *xy, *xz, *yy, *yz, *zz;
};

struct T {
    Oseen3 *oseen;
    struct Tensor O, K;
};

static int tensor_ini(int, struct Tensor*);
static int tensor_fin(struct Tensor*);

int
bi_cortez_ini(real eps, He *he, /**/ T **pq)
{
    T *q;
    int status, n;

    MALLOC(1, &q);
    if (eps <= 0)
	ERR(CO_IO, "eps=%g <= 0", eps);
    status = oseen3_ini(he, eps, &q->oseen);
    if (status != CO_OK)
	ERR(CO_MEMORY, "oseen3_ini failed");
    n = he_nv(he);
    tensor_ini(n, &q->O);
    tensor_ini(n, &q->K);

    *pq = q;
    return CO_OK;
}

int
bi_cortez_argv(char ***p, He *he, /**/ T **pq)
{
    return CO_OK;
}

int
bi_cortez_fin(T *q)
{
    oseen3_fin(q->oseen);
    tensor_fin(&q->O);
    tensor_fin(&q->K);
    FREE(q);
    return CO_OK;
}

int
bi_cortez_update(T *q, He *he, real alpha,
		 const real *x, const real *y, const real *z)
{
    struct Tensor *O, *K;
    int status;

    O = &q->O;
    K = &q->K;
    status = oseen3_apply(q->oseen, he, x, y, z, O->xx, O->xy, O->xz, O->yy, O->yz, O->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_apply failed");
    status = oseen3_stresslet(q->oseen, he, x, y, z, K->xx, K->xy, K->xz, K->yy, K->yz, K->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_stresslet failed");
    return CO_OK;
}

int
bi_cortez_single(T *q, He *he, real al,
		 const real *x, const real *y, const real *z,
		 const real *fx, const real *fy, const real *fz,
		 /*io*/ real *ux, real *uy, real *uz)
{
    struct Tensor *O;
    int n;
    USED(x);
    USED(y);
    USED(z);
    
    O = &q->O;
    n = he_nv(he);
    oseen3_vector_tensor(n, al, fx, fy, fz,
			 O->xx, O->xy, O->xz, O->yy, O->yz, O->zz, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_double(T *q, He *he, real al,
		 const real *x, const real *y, const real *z,
		 const real *ux, const real *uy, const real *uz,
		 /*io*/real *wx, real *wy, real *wz)
{
    int n;
    struct Tensor *K;
    USED(x);
    USED(y);
    USED(z);
    
    K = &q->K;
    n = he_nv(he);
    oseen3_vector_tensor(n, al, ux, uy, uz,
			 K->xx, K->xy, K->xz, K->yy, K->yz, K->zz,
			 wx, wy, wz);
    return CO_OK;
}

static int
tensor_ini(int n, struct Tensor *t)
{
    matrix_ini(n, n, &t->xx);
    matrix_ini(n, n, &t->xy);
    matrix_ini(n, n, &t->xz);
    matrix_ini(n, n, &t->yy);
    matrix_ini(n, n, &t->yz);
    matrix_ini(n, n, &t->zz);
    return CO_OK;
}

static int
tensor_fin(struct Tensor *t)
{
    matrix_fin(t->xx);
    matrix_fin(t->xy);
    matrix_fin(t->xz);
    matrix_fin(t->yy);
    matrix_fin(t->yz);
    matrix_fin(t->zz);
    return CO_OK;
}
