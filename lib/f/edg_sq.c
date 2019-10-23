#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/edg.h"
#include "co/dedg.h"

#include "co/f/edg_sq.h"

#define T HeFEdgSq

struct T {
    int n;
    real *edg;
    real K;
};

static real
sum_sq(int n, real * a)
{
    int i;
    real v;

    v = 0;
    for (i = 0; i < n; i++)
        v += a[i] * a[i];
    return v;
}

int
he_f_edg_sq_ini(real K, He * he, T ** pq)
{
    T *q;
    int n;

    MALLOC(1, &q);
    n = he_ne(he);

    MALLOC(n, &q->edg);

    q->n = n;
    q->K = K;

    *pq = q;
    return CO_OK;
}

int
he_f_edg_sq_argv(char ***p, He * he, T ** pq)
{
    int status;
    real x;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    return he_f_edg_sq_ini(x, he, pq);
}

int
he_f_edg_sq_fin(T * q)
{
    FREE(q->edg);
    FREE(q);
    return CO_OK;
}

int
he_f_edg_sq_e(T * q, /**/ real ** pa)
{
    *pa = q->edg;
    return CO_OK;
}

static void
get_ij(int m, He * he, /**/ int *pi, int *pj)
{
    int h, n, i, j;

    h = he_hdg_edg(he, m);
    n = he_nxt(he, h);
    i = he_ver(he, h);
    j = he_ver(he, n);
    *pi = i;
    *pj = j;
}

static void
get(int m, He * he,
    const real * x, const real * y, const real * z, /**/
    real a[3], real b[3])
{
    int i, j;

    get_ij(m, he, &i, &j);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
}

static void
compute_edg(He * he, const real * x, const real * y, const real * z,
            /**/ real * edg)
{
    real one, a[3], b[3];
    int n, m;

    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b);
        edg[m] = one = edg_abs(a, b);
    }
}

static void
compute_force(He * he, real K, const real * edg,
              const real * x, const real * y, const real * z, /**/
              real * fx, real * fy, real * fz)
{
    int n, m, i, j;
    real a[3], b[3], da[3], db[3], coeff;

    n = he_ne(he);
    for (m = 0; m < n; m++) {
        get_ij(m, he, /**/ &i, &j);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        dedg_abs(a, b, /**/ da, db);
        coeff = 2 * K * edg[m];
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
    }
}

int
he_f_edg_sq_force(T * q, He * he,
                  const real * x, const real * y, const real * z, /**/
                  real * fx, real * fy, real * fz)
{
    int n;
    real *edg, K;

    n = q->n;
    edg = q->edg;
    K = q->K;
    if (he_ne(he) != n)
        ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);
    compute_edg(he, x, y, z, /**/ edg);
    compute_force(he, K, edg, x, y, z, /**/ fx, fy, fz);
    return CO_OK;
}

real
he_f_edg_sq_energy(T * q, He * he,
                   const real * x, const real * y, const real * z)
{
    int n;
    real *edg, v, K;

    n = q->n;
    edg = q->edg;
    K = q->K;

    if (he_ne(he) != n)
        ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);

    compute_edg(he, x, y, z, /**/ edg);
    v = sum_sq(n, edg);
    return K * v;
}
