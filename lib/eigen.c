#include <stdio.h>

#include "real.h"

#include "co/eigen.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/sum.h"
#include "co/ten.h"
#include "co/vec.h"
#include <alg/eig.h>

#define T    Eigen

struct T {
    int n;
    real v[3 * 3];
};


static real
sum(int n, real * a)
{
    return (he_sum_array(n, a));
}


int
eigen_ini(He * he, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->n = he_nv(he);
    *pq = q;
    return (CO_OK);
}


int
eigen_fin(T * q)
{
    FREE(q);
    return (CO_OK);
}


static int
moment(int n, const real * xx, const real * yy, const real * zz,
       /**/ real m[6])
{
    /* moment of inertia tensor */
    int i;
    real x, y, z;

    enum {
        XX,
        XY,
        XZ,
        YY,
        YZ,
        ZZ
    };

    m[XX] = m[XY] = m[XZ] = m[YY] = m[YZ] = m[ZZ] = 0;
    for (i = 0; i < n; i++) {
        x = xx[i];
        y = yy[i];
        z = zz[i];
        m[XX] += y * y + z * z;
        m[XY] += -x * y;
        m[XZ] += -x * z;
        m[YY] += x * x + z * z;
        m[YZ] += -y * z;
        m[ZZ] += x * x + y * y;
    }
    return (CO_OK);
}


static int
to_cm(int n, /**/ real * xx, real * yy, real * zz)
{
    int i;
    real x, y, z;

    x = sum(n, xx) / n;
    y = sum(n, yy) / n;
    z = sum(n, zz) / n;
    for (i = 0; i < n; i++) {
        xx[i] -= x;
        yy[i] -= y;
        zz[i] -= z;
    }
    return (CO_OK);
}


int
eigen_vector(T * q, real * x, real * y, real * z, /**/ real * v)
{
    int n, i;
    real m[6];

    n = q->n;
    v = q->v;
    to_cm(n, /**/ x, y, z);
    moment(n, x, y, z, /**/ m);
    alg_eig_vectors(m, v);

    return CO_OK;
}

int
eigen_vector_surface(T * q, He *he, real * x, real * y, real * z, /**/ real * v)
{
    int n, i;
    real m[6];

    n = q->n;
    v = q->v;
    to_cm(n, /**/ x, y, z);
    moment(n, x, y, z, /**/ m);
    alg_eig_vectors(m, v);

    return CO_OK;
}
