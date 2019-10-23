#include <stdio.h>

#include "real.h"

#include "co/dedg.h"
#include "co/dH.h"
#include "co/dtri.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/sum.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/f/gompper_xin.h"

#define T HeFGompperXin

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

static const real pi = 3.141592653589793115997964;

struct T {
    int nv;
    real Kb, H0, Kad, DA0D;
    real *energy, *fx, *fy, *fz;
    real *gx, *gy, *gz;
    Dh *dh;
    real eng_bend, eng_ad;
    real *H;
};

static real
e_local(real H0, real area, real h)
{
    h = h / area - H0;          /* TODO */
    return h * h;
}

static real
ddh_local(void *p, real area, real h)
{
    real H0;

    H0 = *(real *) p;
    h = h / area - H0;
    return 2 * h;
}

static real
dda_local(void *p, real area, real h)
{
    real H0;

    H0 = *(real *) p;
    h = h / area - H0;
    return -h * (h + 2 * H0);
}


static real
e_global(real Area, real diff)
{
    return diff * diff / Area;
}

static real
ddh_global(void *p, __UNUSED real area_, __UNUSED real h_)
{
    real d_over_A;

    d_over_A = *(real *) p;
    return 2 * d_over_A;
}

static real
dda_global(void *p, __UNUSED real area_, __UNUSED real h_)
{
    real d_over_A;

    d_over_A = *(real *) p;
    return -(d_over_A * d_over_A);
}

static void
zero(int n, real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = 0;
}

static int
plus(int n, const real * a, /*io */ real * b)
{
    int i;

    for (i = 0; i < n; i++)
        b[i] += a[i];
    return CO_OK;
}

static int
scale(real sc, int n, /*io */ real * a)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] *= sc;
    return CO_OK;
}

static int
div(int n, const real * a, const real * b, /*io */ real * c)
{
    int i;

    for (i = 0; i < n; i++) {
        if (b[i] == 0)
            ERR(CO_NUM, "b[%d] == 0", i);
        c[i] = a[i] / b[i];
    }
    return CO_OK;
}

int
he_f_gompper_xin_ini(real Kb, real C0, real Kad, real DA0D, He * he,
                     T ** pq)
{
#define M(n, f) MALLOC(n, &q->f)
#define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    M(nv, energy);
    M(nv, fx);
    M(nv, fy);
    M(nv, fz);
    M(nv, gx);
    M(nv, gy);
    M(nv, gz);
    M(nv, H);

    S(nv);
    S(Kb);
    q->H0 = C0 / 2;
    S(Kad);
    S(DA0D);

    dh_ini(he, &q->dh);

    *pq = q;
    return CO_OK;
#undef S
#undef M
}

int
he_f_gompper_xin_argv(__UNUSED char ***p, __UNUSED He * he,
                      T __UNUSED ** pq)
{
    return CO_OK;
}

int
he_f_gompper_xin_fin(T * q)
{
#define F(x) FREE(q->x)
    dh_fin(q->dh);
    F(energy);
    F(fx);
    F(fy);
    F(fz);
    F(gx);
    F(gy);
    F(gz);
    F(H);
    FREE(q);
    return CO_OK;
#undef F
}

static int
compute_energy(real H0, int n,
               const real * area, const real * h, /**/ real * energy)
{
    int i;

    for (i = 0; i < n; i++) {
        energy[i] = area[i] * e_local(H0, area[i], h[i]);
    }
    return CO_OK;
}

real
he_f_gompper_xin_energy(T * q, He * he,
                        const real * x, const real * y, const real * z)
{
    /* get, set */
#define G(f) f = q->f
    int nv;
    real *energy;
    Dh *dh;
    real Kb, H0, Kad, DA0D;
    real *area, *h, eng_bend, eng_ad, Area, Ha, diff;

    G(Kb);
    G(H0);
    G(Kad);
    G(DA0D);
    G(energy);
    G(dh);

    nv = he_nv(he);

    dh_area_h(dh, he, x, y, z);
    dh_area(dh, &area);
    dh_h(dh, &h);

    compute_energy(H0, nv, area, h, energy);
    scale(2 * Kb, nv, energy);
    eng_bend = he_sum_array(nv, energy);

    Area = he_sum_array(nv, area);
    Ha = he_sum_array(nv, h);
    diff = Ha - DA0D / 2;
    eng_ad = (2 * pi * Kad) * e_global(Area, diff);

    q->eng_bend = eng_bend;
    q->eng_ad = eng_ad;

    return eng_bend + eng_ad;

#undef A
#undef S
}

int
he_f_gompper_xin_force(T * q, He * he,
                       const real * x, const real * y, const real * z, /**/
                       real * hx, real * hy, real * hz)
{
    /* get, set */
#define G(f) f = q->f
    int nv;
    Dh *dh;
    real Kb, H0, Kad, DA0D;
    real *area, *h;
    real *fx, *fy, *fz, *gx, *gy, *gz;
    real Area, Ha, diff, d_over_A, C;
    dHParam param;

    G(Kb);
    G(H0);
    G(Kad);
    G(DA0D);
    G(dh);
    G(fx);
    G(fy);
    G(fz);
    G(gx);
    G(gy);
    G(gz);

    nv = he_nv(he);
    zero(nv, fx);
    zero(nv, fy);
    zero(nv, fz);
    zero(nv, gx);
    zero(nv, gy);
    zero(nv, gz);

    param.dh = ddh_local;
    param.da = dda_local;
    param.p = (void *) &H0;
    dh_force(dh, param, he, x, y, z, /**/ fx, fy, fz);

    dh_area(dh, &area);
    dh_h(dh, &h);

    scale(2 * Kb, nv, fx);
    scale(2 * Kb, nv, fy);
    scale(2 * Kb, nv, fz);
    plus(nv, fx, hx);
    plus(nv, fy, hy);
    plus(nv, fz, hz);

    Area = he_sum_array(nv, area);
    Ha = he_sum_array(nv, h);
    diff = Ha - DA0D / 2;
    d_over_A = diff / Area;

    param.dh = ddh_global;
    param.da = dda_global;
    param.p = (void *) &d_over_A;
    dh_force(dh, param, he, x, y, z, /**/ gx, gy, gz);

    C = 2 * pi * Kad;
    scale(C, nv, gx);
    scale(C, nv, gy);
    scale(C, nv, gz);
    plus(nv, gx, hx);
    plus(nv, gy, hy);
    plus(nv, gz, hz);

    return CO_OK;

#undef A
#undef S
}

int
he_f_gompper_xin_energy_ver(T * q, /**/ real ** pa)
{
    *pa = q->energy;
    return CO_OK;
}

int
he_f_gompper_xin_area_ver(T * q, /**/ real ** pa)
{
    return dh_area(q->dh, pa);
}

int
he_f_gompper_xin_curva_mean_ver(T * q, /**/ real ** pa)
{
#define G(f) f = q->f
    int nv;
    Dh *dh;
    real *H, *area, *h;

    G(nv);
    G(dh);
    G(H);
    dh_area(dh, &area);
    dh_h(dh, &h);

    div(nv, h, area, /**/ H);
    *pa = q->H;

    return CO_OK;
#undef G
}

int
he_f_gompper_xin_norm_ver(T * q, /**/ real ** x, real ** y, real ** z)
{
    return dh_norm(q->dh, x, y, z);
}

real
he_f_gompper_xin_energy_ad(T * q)
{
    return q->eng_ad;
}

real
he_f_gompper_xin_energy_bend(T * q)
{
    return q->eng_bend;
}
