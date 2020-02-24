#include <stddef.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"

#include "co/macro.h"
#include "co/memory.h"
#include "co/strain/3d.h"
#include "co/strain.h"
#include "co/tri.h"
#include "co/util.h"

#define T Strain
#define P StrainParam

typedef int (*TypeGun)(void *, real, real, /**/ real *, real *);
typedef real(*TypeFun) (void *, real, real);

struct T {
    TypeFun F, F1, F2;
    TypeGun G;
    P param;
};

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

static int
G_linear(void *p0, real al, real be, real * a, real * b)
{
#define G(s) s = p->s
    P *p;
    real Ka, mu;

    p = (P *) p0;
    G(Ka);
    G(mu);

    *a = Ka * al * al / 2;
    *b = mu * be;
    return CO_OK;
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

static int
G_lim(void *p0, real al, real be, real * a, real * b)
{
#define G(s) s = p->s
    P *p;
    real Ka, mu, a3, a4, b1, b2;

    p = (P *) p0;
    G(Ka);
    G(mu);
    G(a3);
    G(a4);
    G(b1);
    G(b2);

    *a = Ka / 2 * (al * al + a3 * al * al * al + a4 * al * al * al * al);
    *b = mu * (be + b1 * al * be + b2 * be * be);
    return CO_OK;
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
    if (util_eq(name, "linear")) {
        q->G = G_linear;
        q->F = F_linear;
        q->F1 = F1_linear;
        q->F2 = F2_linear;
    } else if (util_eq(name, "lim")) {
        q->G = G_lim;
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
    int status;
    P *param;
    TypeFun F, F1, F2;

    param = &q->param;
    F = q->F;
    F1 = q->F1;
    F2 = q->F2;
    status = strain_force_3d((void *) param, F, F1, F2, a0, b0, c0, a, b, c, da, db,
                    dc);
    if (status != CO_OK) {
	tri_off(a0, b0, c0, stderr);
	tri_off(a, b, c, stderr);
	ERR(CO_NUM, "strain_force failed");
    }
    return CO_OK;
}

real
strain_energy(T * q, const real a0[3], const real b0[3], const real c0[3],
              const real a[3], const real b[3], const real c[3])
{
    real eng;
    P *param;
    TypeFun F;

    param = &q->param;
    F = q->F;
    strain_energy_3d((void *) param, F, a0, b0, c0, a, b, c, /**/ &eng);
    return eng;
}

int
strain_energy_ab(T * q, const real a0[3], const real b0[3],
                 const real c0[3], const real a[3], const real b[3],
                 const real c[3], real * ea, real * eb)
{
    P *param;
    TypeGun G;

    param = &q->param;
    G = q->G;
    strain_energy_3d_ab((void *) param, G, a0, b0, c0, a, b, c, /**/ ea,
                        eb);
    return CO_OK;
}
