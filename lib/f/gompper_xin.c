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
    real *gx, *gy, *gz;
    real *area; /* from Dh */
    Dh *dh;
};

static real e_local(real H0, real area, real h) {
    h = h/area - H0; /* TODO */
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


static real e_global(real Area, real diff) {
    return diff*diff/Area;
}
static real ddh_global(void *p, __UNUSED real area_, __UNUSED real h_) {
    real d_over_A;
    d_over_A = *(real*)p;
    return 2*d_over_A;
}
static real dda_global(void *p, __UNUSED real area_, __UNUSED real h_) {
    real d_over_A;
    d_over_A = *(real*)p;
    return -(d_over_A*d_over_A);
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
#   define M(n, f) MALLOC(n, &q->f)
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    M(nv, energy);
    M(nv, fx); M(nv, fy); M(nv, fz);
    M(nv, gx); M(nv, gy); M(nv, gz);

    q->Kb = Kb;
    q->H0 = C0/2;
    q->Kad = Kad;
    q->DA0D = DA0D;

    dh_ini(he, &q->dh);

    *pq = q;
    return HE_OK;
#   undef M
}

int he_f_gompper_xin_fin(T *q) {
    dh_fin(q->dh);
    FREE(q->energy);
    FREE(q->fx); FREE(q->fy); FREE(q->fz);
    FREE(q->gx); FREE(q->gy); FREE(q->gz);
    FREE(q);
    return HE_OK;
}

static int compute_energy(real H0, int n,
                           const real *area, const real *h,
                           /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++) {
        energy[i] = area[i]*e_local(H0, area[i], h[i]);
    }
    return HE_OK;
}
real he_f_gompper_xin_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
#   define S(f) q->f = f
    int nv;
    real *energy;
    Dh *dh;
    real Kb, H0, Kad, DA0D;

    real *area, *h, local, global, Area, Ha, diff;

    G(Kb); G(H0); G(Kad); G(DA0D);
    G(energy); G(dh);

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
    global = (2*pi*Kad)*e_global(Area, diff);

    S(area);

    return local + global;

#   undef A
#   undef S
}

int he_f_gompper_xin_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *hx, real *hy, real *hz) {
    /* get, set */
#   define G(f) f = q->f
#   define S(f) q->f = f
    int nv;
    Dh *dh;
    real Kb, H0, Kad, DA0D;
    real *area, *h;
    real *fx, *fy, *fz, *gx, *gy, *gz;
    real Area, Ha, diff, d_over_A, C;
    dHParam param;

    G(Kb); G(H0); G(Kad); G(DA0D);
    G(dh);
    G(fx); G(fy); G(fz);
    G(gx); G(gy); G(gz);

    nv = he_nv(he);
    zero(nv, fx); zero(nv, fy); zero(nv, fz);
    zero(nv, gx); zero(nv, gy); zero(nv, gz);

    param.dh = ddh_local;
    param.da = dda_local;
    param.p  = (void*)&H0;
    dh_force(dh, param, he, x, y, z, /**/ fx, fy, fz);

    dh_area(dh, &area);
    dh_h(dh, &h);

    scale(2*Kb, nv, fx); scale(2*Kb, nv, fy); scale(2*Kb, nv, fz);
    plus(nv, fx, hx); plus(nv, fy, hy); plus(nv, fz, hz);

    Area = he_sum_array(nv, area);
    Ha = he_sum_array(nv, h);
    diff = Ha - DA0D/2;
    d_over_A = diff/Area;

    param.dh = ddh_global;
    param.da = dda_global;
    param.p  = (void*)&d_over_A;
    dh_force(dh, param, he, x, y, z, /**/ gx, gy, gz);

    C = 2*pi*Kad;
    scale(C, nv, gx); scale(C, nv, gy); scale(C, nv, gz);
    plus(nv, gx, hx); plus(nv, gy, hy); plus(nv, gz, hz);

    S(area);

    return HE_OK;

#   undef A
#   undef S
}

int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}

int he_f_gompper_xin_area_ver(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}
