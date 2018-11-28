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

#include "he/f/gompper_xin.h"

#define T HeFGompperXin

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

struct T {
    real Kb, H0;
    real *energy;
    Dh *dh;
};

static real sq(real x) { return x*x; }
static real e(real H0, real area, real h) {
    real H;
    H = h/area; /* TODO */
    return sq(H - H0);
}
static real ddh(void *p, real area, real H) { return   4*H/area; }
static real dda(void *p, real area, real H) { return  -(2*H*H)/(area*area); }
static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static int scale(int n, real sc, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return HE_OK;
}

int he_f_gompper_xin_ini(real Kb, real C0, __UNUSED real Kad, __UNUSED real DA0D, He *he, T **pq) {
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    CALLOC(nv, &q->energy);

    q->Kb = Kb;
    q->H0 = C0/2;

    dh_ini(he, &q->dh);

    *pq = q;
    return HE_OK;
}

int he_f_gompper_xin_fin(T *q) {
    dh_fin(q->dh);
    FREE(q->energy);
    FREE(q);
    return HE_OK;
}

static int compute_energy(real H0, int n,
                           const real *h, const real *area,
                           /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++) {
        energy[i] = area[i]*e(H0, h[i], area[i]);
    }
    return HE_OK;
}
real he_f_gompper_xin_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
#   define A(f) f = q->f
    int nv, i;
    real *energy;
    Dh *dh;
    real Kb, H0;
    
    real *area, *h;

    A(energy); A(Kb); A(H0); A(dh);

    nv = he_nv(he);

    dh_area_h(dh, he, x, y, z);
    dh_area(dh, &area);
    dh_h(dh, &h);

    compute_energy(H0, nv, h, area, energy);

    return he_sum_array(nv, energy);
#   undef A
}

int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}

int he_f_gompper_xin_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *fx_tot, real *fy_tot, real *fz_tot) {
    return HE_OK;
}
