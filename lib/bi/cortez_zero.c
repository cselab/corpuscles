#include <stdio.h>
#include "real.h"
#include "co/argv.h"
#include "co/bi/cortez_zero.h"
#include "co/err.h"
#include "co/he.h"
#include "co/i/matrix.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/oseen3zero.h"
#include "co/tensor.h"

#define T BiCortezZero
static const real pi = 3.141592653589793115997964;
struct T {
    Oseen3Zero *oseen;
    struct Tensor O, K;
    real eps;
};

int
bi_cortez_zero_ini(real eps, He *he, /**/ T **pq)
{
    T *q;
    int status, n;

    MALLOC(1, &q);
    if (eps <= 0)
	ERR(CO_IO, "eps=%g <= 0", eps);
    status = oseen3_zero_ini(he, &q->oseen);
    if (status != CO_OK)
	ERR(CO_MEMORY, "oseen3_zero_ini failed");
    n = he_nv(he);
    q->eps = eps;
    tensor_ini(n, &q->O);
    tensor_ini(n, &q->K);

    *pq = q;
    return CO_OK;
}

int
bi_cortez_zero_argv(char ***p, He *he, /**/ T **pq)
{
    int status;
    real x;
    if ((status = argv_real(p, &x)) != CO_OK)
	return status;
    return bi_cortez_zero_ini(x, he, pq);
}

int
bi_cortez_zero_fin(T *q)
{
    oseen3_zero_fin(q->oseen);
    tensor_fin(&q->O);
    tensor_fin(&q->K);
    FREE(q);
    return CO_OK;
}

int
bi_cortez_zero_update(T *q, He *he, const real *x, const real *y, const real *z)
{
    struct Tensor *O, *K;
    int status, n, i;
    real eps, coeff;

    O = &q->O;
    K = &q->K;
    eps = q->eps;
    status = oseen3_zero_apply(q->oseen, he, x, y, z, O->xx, O->xy, O->xz, O->yy, O->yz, O->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_zero_apply failed");

    n = he_nv(he);
    coeff = 1/(4*pi*eps);
    for (i = 0; i < n; i++) {
	i_matrix_add(n, n, i, i, coeff, O->xx);
	i_matrix_add(n, n, i, i, coeff, O->yy);
	i_matrix_add(n, n, i, i, coeff, O->zz);
    }
    status = oseen3_zero_stresslet(q->oseen, he, x, y, z, K->xx, K->xy, K->xz, K->yy, K->yz, K->zz);
    if (status != CO_OK)
	ERR(CO_NUM, "oseen3_zero_stresslet failed");
    return CO_OK;
}

int
bi_cortez_zero_single(T *q, He *he, real al,
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
    tensor_vector(n, al, fx, fy, fz, O, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_zero_double(T *q, He *he, real al,
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
    tensor_vector(n, al, ux, uy, uz, K, wx, wy, wz);
    return CO_OK;
}
