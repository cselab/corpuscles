#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"
#include "he/dedg.h"
#include "he/sum.h"
#include "he/dH.h"
#include "he/da.h"
#include "he/macro.h"

#include "he/f/%name%.h"

#define T HeF%Name%

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

static const real pi = 3.141592653589793115997964;

struct T {
    real a0, K;
    //%array nv energy
    //%array nv fx fy fz
    Dh *dh;
    Da *da;

    int nv;
    //%array nv H
};

static real e(real area0, real area) {
    real d;
    d = area - area0;
    return d*d;
}
static real ddh_local(void *p, real area, real h) {
    real H0;
    H0 = *(real*)p;
    h = h/area - H0;
    return 2*h;
}
static real dda_local(void *p, real area, real h) {
    real H0;
    H0 = *(real*)p;
    h = h/area - H0;
    return  -h*(h + 2*H0);
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static int plus(int n, const real *a, /*io*/ real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] += a[i];
    return HE_OK;
}
static int scale(real sc, int n, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return HE_OK;
}
static int div(int n, const real *a, const real *b, /*io*/ real *c) {
    int i;
    for (i = 0; i < n; i++) {
        if (b[i] == 0) ERR(HE_NUM, "b[%d] == 0", i);
        c[i] = a[i]/b[i];
    }
    return HE_OK;
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

    dh_ini(he, &q->dh);
    da_ini(he, &q->da);

    *pq = q;
    return HE_OK;
#   undef S
#   undef M
}

int he_f_%name%_fin(T *q) {
#   define F(x) FREE(q->x)
    dh_fin(q->dh);
    da_fin(q->da);
    //%free
    FREE(q);
    return HE_OK;
#   undef F
}

static int compute_energy(int n,
                          real area0, const real *area,
                          /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++)
        energy[i] = e(area0, area[i]);
    return HE_OK;
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
    return he_sum_array(nv, energy);

#   undef A
#   undef S
}

int he_f_%name%_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *hx, real *hy, real *hz) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    Dh *dh;
    real a0, K;
    real *area, *h;
    real *fx, *fy, *fz;
    real Area, Ha, diff, d_over_A, C;
    dHParam param;

    G(a0); G(K);
    G(dh);
    G(fx); G(fy); G(fz);

    nv = he_nv(he);
    zero(nv, fx); zero(nv, fy); zero(nv, fz);

    param.dh = ddh_local;
    param.da = dda_local;
    param.p  = (void*)&K;
    dh_force(dh, param, he, x, y, z, /**/ fx, fy, fz);

    dh_area(dh, &area);
    dh_h(dh, &h);

    C = K/a0;
    scale(C, nv, fx); scale(C, nv, fy); scale(C, nv, fz);
    plus(nv, fx, hx); plus(nv, fy, hy); plus(nv, fz, hz);

    return HE_OK;

#   undef A
#   undef S
}
