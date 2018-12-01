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
#include "he/macro.h"

#include "he/f/area_voronoi.h"

#define T HeFAreaVoronoi

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

static const real pi = 3.141592653589793115997964;

struct T {
    real a0, K;
    real *energy;
    real *fx, *fy, *fz;
    Dh *dh;

    int nv;
    real *H;
};

static real e_local(real area, real h) {
    h = h/area;
    return h*h;
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

int he_f_area_voronoi_ini(real a0, real K, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    M(nv, energy);
    M(nv, fx); M(nv, fy); M(nv, fz);
    M(nv, H);

    S(nv);
    S(a0); S(K);

    dh_ini(he, &q->dh);

    *pq = q;
    return HE_OK;
#   undef S
#   undef M
}

int he_f_area_voronoi_fin(T *q) {
#   define F(x) FREE(q->x)
    dh_fin(q->dh);
    F(energy);
    F(fx); F(fy); F(fz);
    F(H);
    FREE(q);
    return HE_OK;
#   undef F
}

static int compute_energy(int n,
                           const real *area, const real *h,
                           /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++) {
        energy[i] = area[i]*e_local(area[i], h[i]);
    }
    return HE_OK;
}
real he_f_area_voronoi_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    real *energy;
    Dh *dh;
    real a0, K;

    real *area, *h, local, global, Area, Ha, diff;

    G(a0); G(K);
    G(energy); G(dh);

    nv = he_nv(he);

    dh_area_h(dh, he, x, y, z);
    dh_area(dh, &area);
    dh_h(dh, &h);

    compute_energy(nv, area, h, energy);
    scale(K/a0, nv, energy);
    return he_sum_array(nv, energy);

#   undef A
#   undef S
}

int he_f_area_voronoi_force(T *q, He *he,
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
