#include <stdio.h>
#include <math.h>

#include "real.h"
#include "co/dtri.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/sum.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/f/darea.h"
#include "co/f/dvolume.h"
#include "co/f/rvolume.h"

#define T  HeFRvolume
#define TA HeFDarea
#define TV HeFDvolume

static const real pi = 3.141592653589793115997964;

struct T {
    TA *qa;
    TV *qv;
    int n;
    real *ftx, *fty, *ftz;
};


int
he_f_rvolume_ini(He * he, T ** pq)
{
    T *q;
    int nt, nv;

    MALLOC(1, &q);

    nt = he_nt(he);
    nv = he_nv(he);

    he_f_darea_ini(he, &q->qa);
    he_f_dvolume_ini(he, &q->qv);
    q->n = nt;

    MALLOC(nv, &q->ftx);
    MALLOC(nv, &q->fty);
    MALLOC(nv, &q->ftz);

    *pq = q;
    return CO_OK;
}

int
he_f_rvolume_argv(__UNUSED char ***p, He * he, T ** pq)
{
    return he_f_rvolume_ini(he, pq);
}

int
he_f_rvolume_fin(T * q)
{
    he_f_darea_fin(q->qa);
    he_f_dvolume_fin(q->qv);
    FREE(q->ftx);
    FREE(q->fty);
    FREE(q->ftz);
    FREE(q);
    return CO_OK;
}

static real
svolume(real R)
{
    return 4.0 * pi * R * R * R / 3.0;
}

static real
sradius(real A)
{
    return pow(A / 4.0 / pi, 0.5);
}

static void
zero(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = 0;
}

int
he_f_rvolume_force(T * q, He * he,
                   const real * x, const real * y, const real * z, /**/
                   real * fx, real * fy, real * fz)
{

    real A, V, Vs, R;
    real *ftx, *fty, *ftz;
    int Nv, i;
    real coef;
    int n;

    n = q->n;
    ftx = q->ftx;
    fty = q->fty;
    ftz = q->ftz;

    A = he_f_darea_energy(q->qa, he, x, y, z);
    V = he_f_dvolume_energy(q->qv, he, x, y, z);
    R = sradius(A);
    Vs = svolume(R);

    Nv = he_nv(he);
    if (he_nt(he) != n)
        ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    zero(Nv, ftx);
    zero(Nv, fty);
    zero(Nv, ftz);
    he_f_darea_force(q->qa, he, x, y, z, /**/ ftx, fty, ftz);

    coef = -V * R / Vs / Vs / 2.0;

    for (i = 0; i < Nv; i++) {
        fx[i] += coef * ftx[i];
        fy[i] += coef * fty[i];
        fz[i] += coef * ftz[i];
    }

    zero(Nv, ftx);
    zero(Nv, fty);
    zero(Nv, ftz);
    he_f_dvolume_force(q->qv, he, x, y, z, /**/ ftx, fty, ftz);

    coef = 1 / Vs;
    for (i = 0; i < Nv; i++) {
        fx[i] += coef * ftx[i];
        fy[i] += coef * fty[i];
        fz[i] += coef * ftz[i];
    }
    return CO_OK;
}

real
he_f_rvolume_energy(T * q, He * he,
                    const real * x, const real * y, const real * z)
{
    real A, V, Vs, R;
    int n;

    n = q->n;

    A = he_f_darea_energy(q->qa, he, x, y, z);
    V = he_f_dvolume_energy(q->qv, he, x, y, z);
    R = sradius(A);
    Vs = svolume(R);

    if (he_nt(he) != n)
        ERR(CO_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    return V / Vs;

}
