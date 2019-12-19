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
#include "co/dH.h"
#include "co/macro.h"

#include "co/f/%name%.h"

#define T HeF%Name%

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

static const real pi = 3.141592653589793115997964;

struct T {
    real Kb, H0, Kad, DA0D;
    //%array nv energy
    //%array nv fx fy fz
    //%array nv gx gy gz
    Dh *dh;

    int nv;
    //%array nv H
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
    return CO_OK;
}
static int scale(real sc, int n, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return CO_OK;
}
static int div(int n, const real *a, const real *b, /*io*/ real *c) {
    int i;
    for (i = 0; i < n; i++) {
        if (b[i] == 0) ERR(CO_NUM, "b[%d] == 0", i);
        c[i] = a[i]/b[i];
    }
    return CO_OK;
}

int he_f_%name%_ini(real Kb, real C0, real Kad, real DA0D, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    //%malloc

    S(nv);
    S(Kb); q->H0 = C0/2; S(Kad); S(DA0D);

    dh_ini(he, &q->dh);

    *pq = q;
    return CO_OK;
#   undef S
#   undef M
}

int he_f_%name%_fin(T *q) {
#   define F(x) FREE(q->x)
    dh_fin(q->dh);
    //%free
    FREE(q);
    return CO_OK;
#   undef F
}

static int compute_energy(real H0, int n,
                           const real *area, const real *h,
                           /**/ real *energy) {
    int i;
    for (i = 0; i < n; i++) {
        energy[i] = area[i]*e_local(H0, area[i], h[i]);
    }
    return CO_OK;
}
real he_f_%name%_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
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
    local = sum_array(nv, energy);

    Area = sum_array(nv, area);
    Ha = sum_array(nv, h);
    diff = Ha - DA0D/2;
    global = (2*pi*Kad)*e_global(Area, diff);

    return local + global;

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

    Area = sum_array(nv, area);
    Ha = sum_array(nv, h);
    diff = Ha - DA0D/2;
    d_over_A = diff/Area;

    param.dh = ddh_global;
    param.da = dda_global;
    param.p  = (void*)&d_over_A;
    dh_force(dh, param, he, x, y, z, /**/ gx, gy, gz);

    C = 2*pi*Kad;
    scale(C, nv, gx); scale(C, nv, gy); scale(C, nv, gz);
    plus(nv, gx, hx); plus(nv, gy, hy); plus(nv, gz, hz);

    return CO_OK;

#   undef A
#   undef S
}
