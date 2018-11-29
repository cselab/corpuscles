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

static const real pi = 3.141592653589793115997964;

struct T {
    real Kb, H0, Kad, DA0D;
    real *energy, *fx, *fy, *fz;
    Dh *dh;
};

static real e(real H0, real area, real h) {
    h = h/area - H0; /* TODO */
    return h*h;
}
static real ddh(void *p, real area, real h) {
    real H0;
    H0 = *(real*)p;
    h = h/area - H0;
    return 2*h;
}
static real dda(void *p, real area, real h) {
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
int he_f_gompper_xin_ini(real Kb, real C0, real Kad, real DA0D, He *he, T **pq) {
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    MALLOC(nv, &q->energy);
    MALLOC(nv, &q->fx); MALLOC(nv, &q->fy); MALLOC(nv, &q->fz);

    q->Kb = Kb;
    q->H0 = C0/2;
    q->Kad = Kad;
    q->DA0D = DA0D;

    dh_ini(he, &q->dh);

    *pq = q;
    return HE_OK;
}

int he_f_gompper_xin_fin(T *q) {
    dh_fin(q->dh);
    FREE(q->energy);
    FREE(q->fx); FREE(q->fy); FREE(q->fz);
    FREE(q);
    return HE_OK;
}

static int compute_energy(real H0, int n,
                           const real *area, const real *h,
                           /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++) {
        energy[i] = area[i]*e(H0, area[i], h[i]);
    }
    return HE_OK;
}
real he_f_gompper_xin_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
#   define A(f) f = q->f
    int nv;
    real *energy;
    Dh *dh;
    real Kb, H0, Kad, DA0D;

    real *area, *h, local, global, Area, Ha, diff;

    A(Kb); A(H0); A(Kad); A(DA0D);
    A(energy); A(dh);

    nv = he_nv(he);

    dh_area_h(dh, he, x, y, z);
    dh_area(dh, &area);
    dh_h(dh, &h);

    compute_energy(H0, nv, area, h, energy);
    scale(2*Kb, nv, energy);
    local = he_sum_array(nv, energy);

    Area = he_sum_array(nv, area);
    Ha = he_sum_array(nv, h);
    diff = Ha - DA0D/2;
    global = (2*pi*Kad)*diff*diff/Area;
    return local + global;
#   undef A
}

int he_f_gompper_xin_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *hx, real *hy, real *hz) {
#   define A(f) f = q->f
    int nv;
    Dh *dh;
    real Kb, H0;
    real *area, *h;
    real *fx, *fy, *fz;
    dHParam param;
    A(Kb); A(H0); A(dh); A(fx); A(fy); A(fz);

    param.dh = ddh;
    param.da = dda;
    param.p  = (void*)&H0;
    
    nv = he_nv(he);
    zero(nv, fx); zero(nv, fy); zero(nv, fz);
    
    dh_force(dh, param, he, x, y, z, /**/ fx, fy, fz);

    scale(2*Kb, nv, fx);
    scale(2*Kb, nv, fy);
    scale(2*Kb, nv, fz);

    plus(nv, fx, hx);
    plus(nv, fy, hy);
    plus(nv, fz, hz);

    return HE_OK;
#   undef A
}

int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
