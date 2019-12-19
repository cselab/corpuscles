#include <stddef.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"

#include "co/memory.h"
#include "co/macro.h"
#include "co/strain/3d.h"
#include "co/util.h"
#include "co/strain.h"

#define T Strain
#define P StrainParam

typedef real(*TypeFun) (void *, real, real);

struct T {
    TypeFun F, F1, F2;
    P param;
};

static real
sq(real x)
{
    return x * x;
}

static real
F_skalak(void *p0, real I1, real I2)
{
    P *p;
    real Ks, Ka, I1s, I2s;

    p = (P *) p0;
    Ks = p->Ks;
    Ka = p->Ka;
    I1s = 2 * I1;
    I2s = 4 * I2 + 2 * I1;
    return Ks * (sq(I1s) / 2 + I1s - I2s) / 4 + Ka * sq(I2s) / 8;
}

static real
F1_skalak(void *p0, real I1, __UNUSED real I2)
{
    P *p;
    real Ks, Ka;

    p = (P *) p0;
    Ks = p->Ks;
    Ka = p->Ka;
    return I1 * Ks + Ka / 4;
}

static real
F2_skalak(void *p0, __UNUSED real I1, __UNUSED real I2)
{
    P *p;
    real Ka, Ks;

    p = (P *) p0;
    Ka = p->Ka;
    Ks = p->Ks;
    return -(2 * Ks - Ka) / 2;
}

static real
F_evans(void *p0, __UNUSED real I1, __UNUSED real I2)
{
    P *p;
    real Ks, Ka, I1s, I2s;

    p = (P *) p0;
    Ka = p->Ka;
    Ks = p->Ks;
    I1s = I2s = 0;
    return Ks * (sq(I1s) / 2 + I1s - I2s) / 4 + Ka * sq(I2s) / 8;
}

static real
F1_evans(void *p0, real I1, __UNUSED real I2)
{
    P *p;
    real Ks, Ka;

    p = (P *) p0;
    Ks = p->Ks;
    Ka = p->Ka;
    return I1 * Ks + Ka / 4;
}

static real
F2_evans(void *p0, __UNUSED real I1, __UNUSED real I2)
{
    P *p;
    real Ka, Ks;

    p = (P *) p0;
    Ka = p->Ka;
    Ks = p->Ks;
    return -(2 * Ks - Ka) / 2;
}

static real
F_linear(void *p0, real al, real be)
{
#define G(s) s = p->s
    real A, B;
    P *p;
    real Ka, mu;

    p = (P *) p0;
    G(Ka);
    G(mu);

    A = Ka * al * al / 2;
    B = mu * be;
    return A + B;
#undef G
}

static real
F1_linear(void *p0, real al, __UNUSED real be)
{
#define G(s) s = p->s
    P *p;
    real Ka;

    p = (P *) p0;
    G(Ka);
    return Ka * al;
#undef G
}

static real
F2_linear(__UNUSED void *p0, __UNUSED real al, __UNUSED real be)
{
#define G(s) s = p->s
    P *p;
    real mu;

    p = (P *) p0;
    G(mu);
    return mu;
#undef G
}

static real
F_lim(void *p0, real al, real be)
{
#define G(s) s = p->s
    real A, B;
    P *p;
    real Ka, mu, a3, a4, b1, b2;

    p = (P *) p0;
    G(Ka);
    G(mu);
    G(a3);
    G(a4);
    G(b1);
    G(b2);

    A = Ka / 2 * (al * al + a3 * al * al * al + a4 * al * al * al * al);
    B = mu * (be + b1 * al * be + b2 * be * be);
    return A + B;
#undef G
}

static real
F1_lim(void *p0, real al, real be)
{
#define G(s) s = p->s
    real dA, dB;
    P *p;
    real Ka, mu, a3, a4, b1;

    p = (P *) p0;
    G(Ka);
    G(mu);
    G(a3);
    G(a4);
    G(b1);
    dA = Ka / 2 * (2 * al + 3 * a3 * al * al + 4 * a4 * al * al * al);
    dB = mu * b1 * be;
    return dA + dB;
#undef G
}

static real
F2_lim(void *p0, real al, real be)
{
#define G(s) s = p->s
    real dB;
    P *p;
    real mu, b1, b2;

    p = (P *) p0;
    G(mu);
    G(b1);
    G(b2);
    dB = mu * (1 + 2 * b2 * be + al * b1);
    return dB;
#undef G
}

int
strain_ini(const char *name, P param, /**/ T ** pq)
{
    T *q;

    MALLOC(1, &q);
    if (util_eq(name, "skalak")) {
        q->F = F_skalak;
        q->F1 = F1_skalak;
        q->F2 = F2_skalak;
    } else if (util_eq(name, "evans")) {
        q->F = F_evans;
        q->F1 = F1_evans;
        q->F2 = F2_evans;
    } else if (util_eq(name, "linear")) {
        q->F = F_linear;
        q->F1 = F1_linear;
        q->F2 = F2_linear;
    } else if (util_eq(name, "lim")) {
        q->F = F_lim;
        q->F1 = F1_lim;
        q->F2 = F2_lim;
    } else
        ERR(CO_INDEX, "unknown strain model: '%s'", name);
    q->param = param;
    *pq = q;
    return CO_OK;
}

int
strain_fin(T * q)
{
    FREE(q);
    return CO_OK;
}

int
strain_force(T * q,
             const real a0[3], const real b0[3], const real c0[3],
             const real a[3], const real b[3], const real c[3], /**/
             real da[3], real db[3], real dc[3])
{
    P *param;
    TypeFun F, F1, F2;

    param = &q->param;
    F = q->F;
    F1 = q->F1;
    F2 = q->F2;
    strain_force_3d((void *) param, F, F1, F2, a0, b0, c0, a, b, c, da, db,
                    dc);
    return CO_OK;
}

real
strain_energy(T * q, const real a0[3], const real b0[3], const real c0[3],
              const real a[3], const real b[3], const real c[3])
{
    real eng, deng;
    P *param;
    TypeFun F;

    param = &q->param;
    F = q->F;
    strain_energy_3d((void *) param, F, a0, b0, c0, a, b, c, /**/ &eng,
                     &deng);
    return eng;
}
