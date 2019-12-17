#include <stdio.h>

#include "real.h"
#include "co/argv.h"
#include "co/dedg.h"
#include "co/edg.h"
#include "co/err.h"
#include "co/he.h"
#include "co/kernel.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/vec.h"

#include "co/f/repel.h"

#define T HeFRepel
#define BEGIN                                                   \
    for (i = 0; i < n; i++) {					\
    he_ring(he, i, &nring, &ring);				\
    for (j = 0; j < n; j++) {					\
    if (j == i)							\
	continue;						\
    InRing = 0;							\
    for (k = 0; k < nring; k++)					\
	if (ring[k] == j)					\
	    InRing = 1;						\
    if (InRing)							\
	continue;						\

#define END } }

struct T {
    int n;
    Kernel *kernel;
    real cutoff;
    real K;
};

static real energy_ij(T *, int, int, const real *, const real *, const real *);
static real force_ij(T *, int, int, const real *, const real *, const real *, real[3]);

int
he_f_repel_ini(real cutoff, real K, He * he, T ** pq)
{
    USED(he);
    T *q;
    MALLOC(1, &q);
    q->cutoff = cutoff;
    q->K = K;
    q->n = he_nv(he);
    kernel_ini(KERNEL_2D, KERNEL_YANG, &q->kernel);
    *pq = q;
    return CO_OK;
}

int
he_f_repel_argv(char ***p, He * he, T ** pq)
{
    int status;
    real x;
    real y;

    if ((status = argv_real(p, &x)) != CO_OK)
        return status;
    if ((status = argv_real(p, &y)) != CO_OK)
        return status;    
    return he_f_repel_ini(x, y, he, pq);
}

int
he_f_repel_fin(T * q)
{
    kernel_fin(q->kernel);
    FREE(q);
    return CO_OK;
}

int
he_f_repel_force(T * q, He * he,
                 const real * x, const real * y, const real * z, /**/
                 real * fx, real * fy, real * fz)
{
    int n;
    real K;
    real f0[3];
    int i;
    int j;
    int nring;
    int *ring;
    int InRing;
    int k;

    n = q->n;
    K = q->K;
    if (he_ne(he) != n)
        ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);
    BEGIN {
	force_ij(q, i, j, x, y, z, f0);
	vec_scalar_append(f0, K, i, fx, fy, fz);
    END }
    return CO_OK;
}

real
he_f_repel_energy(T * q, He * he,
                  const real * x, const real * y, const real * z)
{
    int i;
    int InRing;
    int j;
    int k;
    int n;
    int nring;
    int *ring;
    real eng;
    real K;
    
    n = q->n;
    K = q->K;
    if (he_ne(he) != n)
        ERR(CO_INDEX, "he_ne(he)=%d != n = %d", he_ne(he), n);
    eng = 0;
    BEGIN {
	eng += energy_ij(q, i, j, x, y, z);
    END }
    return K * eng;
}

static real
energy_ij(T * q, int i, int j, const real * x, const real * y, const real * z)
{
    real a[3];
    real b[3];
    real dist;
    real cutoff;
    real dw;
    Kernel *kernel;

    cutoff = q->cutoff;
    kernel = q->kernel;
    vec_get2(i, j, x, y, z, a, b);
    dist = edg_abs(a, b);
    return kernel_dw(kernel, cutoff, dist);
}

static real
force_ij(T * q, int i, int j, const real * x, const real * y, const real * z, real f[3])
{
    real a[3];
    real b[3];
    real dist;
    real cutoff;
    real dw;
    Kernel *kernel;

    cutoff = q->cutoff;
    kernel = q->kernel;
    vec_get2(i, j, x, y, z, a, b);
    dist = edg_abs(a, b);
    dw = kernel_dw(kernel, cutoff, dist);
    vec_minus(a, b, f);
    vec_scale(dw, f);
    return edg_abs(a, b);
}
