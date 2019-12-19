#include <stdio.h>

#include "real.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/dtri.h"
#include "co/dedg.h"
#include "co/sum.h"
#include "co/da.h"
#include "co/macro.h"

#include "co/f/%name%.h"

#define T HeF%Name%

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

struct T {
    real a0, K;
    //%array nv energy
    //%array nv fx fy fz
    Da *da;

    int nv;
    //%array nv H
};

static real e(real area0, real area) {
    real d;
    d = area - area0;
    return d*d;
}
static real dda(void *p, real area) {
    real area0, d;
    area0 = *(real*)p;
    d = area - area0;
    return 2*d;
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static int plus(int n, const real *a, /*io*/ real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] += a[i];
    return CO_OK;
}
static int scale(real sc, int n, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return CO_OK;
}

int he_f_%name%_ini(real a0, real K, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    //%malloc

    S(nv);
    S(a0); S(K);

    da_ini(he, &q->da);

    *pq = q;
    return CO_OK;
#   undef S
#   undef M
}

int he_f_%name%_fin(T *q) {
#   define F(x) FREE(q->x)
    da_fin(q->da);
    //%free
    FREE(q);
    return CO_OK;
#   undef F
}

static int compute_energy(int n,
                          real area0, const real *area,
                          /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++)
        energy[i] = e(area0, area[i]);
    return CO_OK;
}
real he_f_%name%_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    real *energy;
    Da *da;
    real a0, K;

    real *area;

    G(a0); G(K);
    G(energy); G(da);

    nv = he_nv(he);

    da_compute_area(da, he, x, y, z);
    da_area(da, &area);

    compute_energy(nv, a0, area, /**/ energy);
    scale(K/a0, nv, energy);
    return sum_array(nv, energy);

#   undef A
#   undef S
}

int he_f_%name%_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *hx, real *hy, real *hz) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    real a0, K;
    real *fx, *fy, *fz;
    real C;
    Da *da;
    dAParam param;

    G(a0); G(K);
    G(da);
    G(fx); G(fy); G(fz);

    nv = he_nv(he);
    zero(nv, fx); zero(nv, fy); zero(nv, fz);

    param.da = dda;
    param.p  = (void*)&a0;
    da_force(da, param, he, x, y, z, /**/ fx, fy, fz);

    C = K/a0;
    scale(C, nv, fx); scale(C, nv, fy); scale(C, nv, fz);
    plus(nv, fx, hx); plus(nv, fy, hy); plus(nv, fz, hz);

    return CO_OK;
#   undef A
#   undef S
}
