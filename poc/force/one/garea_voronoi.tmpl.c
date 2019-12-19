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
    real A0, K;
    //%array nv fx fy fz
    Da *da;

    int nv;
    //%array nv H
};

static real dda(__UNUSED void *p, __UNUSED real area) {
    return 1;
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

int he_f_%name%_ini(real A0, real K, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    //%malloc

    S(nv);
    S(A0); S(K);

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

real he_f_%name%_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    Da *da;
    real A0, K;
    real C, A, *area, d;

    G(A0); G(K);
    G(da);

    nv = he_nv(he);

    da_compute_area(da, he, x, y, z);
    da_area(da, &area);

    A = sum_array(nv, area);
    d = A - A0;
    C = K/A0;
    return C*d*d;
#   undef A
#   undef S
}

int he_f_%name%_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *hx, real *hy, real *hz) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    real A0, K;
    real *fx, *fy, *fz;
    real *area;
    real A, C;
    Da *da;
    dAParam param;

    G(A0); G(K);
    G(da);
    G(fx); G(fy); G(fz);

    nv = he_nv(he);
    zero(nv, fx); zero(nv, fy); zero(nv, fz);

    param.da = dda;
    da_force(da, param, he, x, y, z, /**/ fx, fy, fz);
    da_area(da, &area);

    A = sum_array(nv, area);
    C = 2*(K/A0)*(A - A0);
    scale(C, nv, fx); scale(C, nv, fy); scale(C, nv, fz);
    plus(nv, fx, hx); plus(nv, fy, hy); plus(nv, fz, hz);

    return CO_OK;
#   undef A
#   undef S
}
