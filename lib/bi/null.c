#include <stdio.h>
#include "real.h"
#include "co/array.h"
#include "co/bi/null.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"

#define T BiNull
struct T {
};

int
bi_null_ini(He * he, /**/ T ** pq)
{
    T *q;

    USED(he);
    MALLOC(1, &q);
    *pq = q;
    return CO_OK;
}

int
bi_null_argv(char ***p, He * he, /**/ T ** pq)
{
    USED(p);
    return bi_null_ini(he, pq);
}

int
bi_null_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
bi_null_update(T * q, He * he, const real * x, const real * y,
               const real * z)
{
    USED(q);
    USED(he);
    USED(x);
    USED(y);
    USED(z);
    return CO_OK;
}

int
bi_null_single(T * q, He * he, real al,
               const real * x, const real * y, const real * z,
               const real * fx, const real * fy, const real * fz,
               /*io */ real * ux, real * uy, real * uz)
{
    USED(q);
    USED(he);
    USED(al);
    USED(x);
    USED(y);
    USED(z);
    USED(fx);
    USED(fy);
    USED(fz);
    return CO_OK;
}

int
bi_null_double(T * q, He * he, real al,
               const real * x, const real * y, const real * z,
               const real * ux, const real * uy, const real * uz,
               /*io */ real * wx, real * wy, real * wz)
{
    USED(x);
    USED(y);
    USED(z);
    int n;

    n = he_nv(he);
    return array_axpy3(n, al, ux, uy, uz, wx, wy, wz);
}

int
bi_null_single_velocity(T * q, He * he,
                        const real * x, const real * y,
                        const real * z, const real * fx,
                        const real * fy, const real * fz,
                        const real r[3], /**/ real v[3])
{
    ERR(CO_NUM, "not implimented");
}

int
bi_null_double_velocity(T * q, He * he,
                        const real * x, const real * y,
                        const real * z, const real * ux,
                        const real * uy, const real * uz,
                        const real r[3], /**/ real v[3])
{
    ERR(CO_NUM, "not implimented");
}
