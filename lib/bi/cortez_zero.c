#include <stdio.h>
#include "real.h"
#include "co/argv.h"
#include "co/bi/cortez_zero.h"
#include "co/bi/self_circle.h"
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
    BiSelfCircle *self;
    Oseen3Zero *oseen;
    struct Tensor O, K;
    int KReady;
};

int
bi_cortez_zero_ini(He * he, /**/ T ** pq)
{
    T *q;
    int status, n;

    MALLOC(1, &q);
    status = oseen3_zero_ini(he, &q->oseen);
    if (status != CO_OK)
        ERR(CO_MEMORY, "oseen3_zero_ini failed");
    status = bi_self_circle_ini(he, &q->self);
    if (status != CO_OK)
        ERR(CO_MEMORY, "bi_self_circle_ini failed");
    n = he_nv(he);
    tensor_ini(n, &q->O);
    tensor_ini(n, &q->K);
    q->KReady = 0;
    *pq = q;
    return CO_OK;
}

int
bi_cortez_zero_argv(char ***p, He * he, /**/ T ** pq)
{
    return bi_cortez_zero_ini(he, pq);
}

int
bi_cortez_zero_fin(T * q)
{
    oseen3_zero_fin(q->oseen);
    bi_self_circle_fin(q->self);
    tensor_fin(&q->O);
    tensor_fin(&q->K);
    FREE(q);
    return CO_OK;
}

int
bi_cortez_zero_update(T * q, He * he, const real * x, const real * y,
                      const real * z)
{
    struct Tensor *O, *K;
    int status, n, i;
    real coeff;

    O = &q->O;
    K = &q->K;
    status =
        oseen3_zero_apply(q->oseen, he, x, y, z, O->xx, O->xy, O->xz,
                          O->yy, O->yz, O->zz);
    if (status != CO_OK)
        ERR(CO_NUM, "oseen3_zero_apply failed");
    status = bi_self_circle_update(q->self, he, x, y, z);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_update failed");
    q->KReady = 0;
    return CO_OK;
}

int
bi_cortez_zero_single(T * q, He * he, real al,
                      const real * x, const real * y, const real * z,
                      const real * fx, const real * fy, const real * fz,
                      /*io */ real * ux, real * uy, real * uz)
{
    struct Tensor *O;
    int n, status;

    USED(x);
    USED(y);
    USED(z);

    O = &q->O;
    n = he_nv(he);
    status =
        bi_self_circle_single(q->self, he, al, x, y, z, fx, fy, fz, ux, uy,
                              uz);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_single failed");
    tensor_vector(n, al, fx, fy, fz, O, ux, uy, uz);
    return CO_OK;
}

int
bi_cortez_zero_double(T * q, He * he, real al,
                      const real * x, const real * y, const real * z,
                      const real * ux, const real * uy, const real * uz,
                      /*io */ real * wx, real * wy, real * wz)
{
    int n, status;
    struct Tensor *K;

    USED(x);
    USED(y);
    USED(z);

    K = &q->K;
    if (q->KReady == 0) {
        status =
            oseen3_zero_stresslet(q->oseen, he, x, y, z, K->xx, K->xy,
                                  K->xz, K->yy, K->yz, K->zz);
        if (status != CO_OK)
            ERR(CO_NUM, "oseen3_zero_stresslet failed");
        q->KReady = 1;
    }
    n = he_nv(he);
    bi_self_circle_double(q->self, he, al, x, y, z, ux, uy, uz, wx, wy,
                          wz);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_double failed");
    tensor_vector(n, al, ux, uy, uz, K, wx, wy, wz);
    return CO_OK;
}
