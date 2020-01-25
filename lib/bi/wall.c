#include <stdio.h>
#include "real.h"
#include "co/argv.h"
#include "co/bi/wall.h"
#include "co/bi/self_circle.h"
#include "co/err.h"
#include "co/he.h"
#include "co/i/matrix.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/green/3wall.h"
#include "co/tensor3x3.h"
#include "co/tensor.h"

#define T BiWall
struct T {
    BiSelfCircle *self;
    Green3Wall *green;
    struct Tensor3x3 O;
    struct Tensor3x3 K;
    int KReady;
};

int
bi_wall_ini(real w, He * he, /**/ T ** pq)
{
    T *q;
    int status, n;

    MALLOC(1, &q);
    status = green3_wall_ini(he, w, &q->green);
    if (status != CO_OK)
        ERR(CO_MEMORY, "green3_wall_ini failed");
    status = bi_self_circle_ini(he, &q->self);
    if (status != CO_OK)
        ERR(CO_MEMORY, "bi_self_circle_ini failed");
    n = he_nv(he);
    tensor3x3_ini(n, &q->O);
    tensor3x3_ini(n, &q->K);
    q->KReady = 0;
    *pq = q;
    return CO_OK;
}

int
bi_wall_argv(char ***p, He * he, /**/ T ** pq)
{
    int status;
    real x;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    return bi_wall_ini(x, he, pq);
}

int
bi_wall_fin(T * q)
{
    green3_wall_fin(q->green);
    bi_self_circle_fin(q->self);
    tensor3x3_fin(&q->O);
    tensor3x3_fin(&q->K);
    FREE(q);
    return CO_OK;
}

int
bi_wall_update(T * q, He * he, const real * x, const real * y,
               const real * z)
{
    struct Tensor3x3 *O;
    int status;

    O = &q->O;
    status = green3_wall_apply(q->green, he, x, y, z, O);
    if (status != CO_OK)
        ERR(CO_NUM, "green3_wall_apply failed");
    status = bi_self_circle_update(q->self, he, x, y, z);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_update failed");
    q->KReady = 0;
    return CO_OK;
}

int
bi_wall_single(T * q, He * he, real al,
               const real * x, const real * y, const real * z,
               const real * fx, const real * fy, const real * fz,
               /*io */ real * ux, real * uy, real * uz)
{
    struct Tensor3x3 *O;
    int n;
    int status;

    O = &q->O;
    n = he_nv(he);
    status =
        bi_self_circle_single(q->self, he, al, x, y, z, fx, fy, fz, ux, uy,
                              uz);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_single failed");
    tensor3x3_vector(n, al, fx, fy, fz, O, ux, uy, uz);
    return CO_OK;
}

int
bi_wall_double(T * q, He * he, real al,
               const real * x, const real * y, const real * z,
               const real * ux, const real * uy, const real * uz,
               /*io */ real * wx, real * wy, real * wz)
{
    int n, status;
    struct Tensor3x3 *K;
    K = &q->K;
    if (q->KReady == 0) {
        status =
           green3_wall_stresslet(q->green, he, x, y, z, K);
        if (status != CO_OK)
            ERR(CO_NUM, "green3_wall_stresslet failed");
        q->KReady = 1;
    }
    n = he_nv(he);
    bi_self_circle_double(q->self, he, al, x, y, z, ux, uy, uz, wx, wy,
                          wz);
    if (status != CO_OK)
        ERR(CO_NUM, "bi_self_circle_double failed");
    tensor3x3_vector(n, al, ux, uy, uz, K, wx, wy, wz);
    return CO_OK;
}

int
bi_wall_single_velocity(T * q, He * he,
                        const real * x, const real * y,
                        const real * z, const real * fx,
                        const real * fy, const real * fz,
                        const real r[3], /**/ real v[3])
{
    ERR(CO_NUM, "not implimented");
//    return green3_wall_single_velocity(q->green, he, x, y, z, fx, fy, fz,
//                                       r, v);
}

int
bi_wall_double_velocity(T * q, He * he,
                        const real * x, const real * y,
                        const real * z, const real * ux,
                        const real * uy, const real * uz,
                        const real r[3], /**/ real v[3])
{
    ERR(CO_NUM, "not implimented");
//    return green3_wall_double_velocity(q->green, he, x, y, z, ux, uy, uz,
//                                       r, v);
}
