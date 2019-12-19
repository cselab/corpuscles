#include <stdio.h>

#include "real.h"

#include "co/dtri.h"
#include "co/err.h"
#include "co/f/dvolume.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/sum.h"
#include "co/tri.h"
#include "co/vec.h"

#define T HeFDvolume

struct T {
    int n;
    real *volume;
};

static real
sum(int n, real * a)
{
    return sum_array(n, a);
}

int
he_f_dvolume_ini(He * he, T ** pq)
{
    T *q;
    int n;

    MALLOC(1, &q);

    n = he_nt(he);
    MALLOC(n, &q->volume);
    q->n = n;

    *pq = q;
    return CO_OK;
}

int
he_f_dvolume_argv(__UNUSED char ***p, He * he, T ** pq)
{
    return he_f_dvolume_ini(he, pq);
}

int
he_f_dvolume_fin(T * q)
{
    FREE(q->volume);
    FREE(q);
    return CO_OK;
}

int
he_f_dvolume_v(T * q, /**/ real ** pa)
{
    *pa = q->volume;
    return CO_OK;
}

static void
get_ijk(int t, He * he, /**/ int *pi, int *pj, int *pk)
{
    int h, n, nn, i, j, k;

    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    i = he_ver(he, h);
    j = he_ver(he, n);
    k = he_ver(he, nn);
    *pi = i;
    *pj = j;
    *pk = k;
}

static void
get(int t, He * he,
    const real * x, const real * y, const real * z, /**/
    real a[3], real b[3], real c[3])
{
    int i, j, k;

    get_ijk(t, he, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}

static void
compute_volume(He * he, const real * x, const real * y, const real * z,
               /**/ real * volume)
{
    real a[3], b[3], c[3];
    int n, t;

    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get(t, he, x, y, z, /**/ a, b, c);
        volume[t] = tri_volume(a, b, c);
    }
}

static void
compute_force(He * he, const real * x, const real * y, const real * z, /**/
              real * fx, real * fy, real * fz)
{
    int n, t, i, j, k;
    real a[3], b[3], c[3], da[3], db[3], dc[3];

    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        dtri_volume(a, b, c, /**/ da, db, dc);
        vec_append(da, i, /**/ fx, fy, fz);
        vec_append(db, j, /**/ fx, fy, fz);
        vec_append(dc, k, /**/ fx, fy, fz);
    }
}

int
he_f_dvolume_force(T * q, He * he,
                   const real * x, const real * y, const real * z, /**/
                   real * fx, real * fy, real * fz)
{
    int n;
    real *volume;

    n = q->n;
    volume = q->volume;
    if (he_nt(he) != n)
        ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);
    compute_volume(he, x, y, z, /**/ volume);
    compute_force(he, x, y, z, /**/ fx, fy, fz);
    return CO_OK;
}

real
he_f_dvolume_energy(T * q, He * he,
                    const real * x, const real * y, const real * z)
{
    int n;
    real *volume;

    n = q->n;
    volume = q->volume;

    if (he_nt(he) != n)
        ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_volume(he, x, y, z, /**/ volume);
    return sum(n, volume);
}
