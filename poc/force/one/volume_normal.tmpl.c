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
#include "co/volume.h"

#include "co/f/%name%.h"

#define T HeF%Name%

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

struct T {
    real v0, K;
    Dh *dh;

    int nv;
    //%array nv H
};

static int axypz(int n, real a, const real *x, const real *y,
                 /**/ real *z) {
    int i;
    for (i = 0; i < n; i++)
        z[i] += a*x[i]*y[i];
    return CO_OK;
}
static int plus(int n, const real *a, /*io*/ real *b) {
    int i;
    for (i = 0; i < n; i++)
        b[i] += a[i];
    return CO_OK;
}

int he_f_%name%_ini(real v0, real K, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    //%malloc

    S(nv);
    S(v0); S(K);

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

real he_f_%name%_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
    real v0, K;
    real C, V, d;

    G(v0); G(K);

    V = volume_tri(he, x, y, z);
    d = V - v0;
    C = K/v0;
    return C*d*d;
#   undef A
#   undef S
}

int he_f_%name%_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *fx, real *fy, real *fz) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    real v0, K;
    real *area;
    real *nx, *ny, *nz;
    real C, V, d;
    Dh *dh;

    G(v0); G(K);
    G(dh);

    nv = he_nv(he);

    dh_area_h(dh, he, x, y, z);
    dh_area(dh, &area);
    dh_norm(dh, &nx, &ny, &nz);

    V = volume_tri(he, x, y, z);
    d = V - v0;
    C = -2*(K/v0)*d; /* TODO: invert nomral */
    axypz(nv, C, area, nx, fx);
    axypz(nv, C, area, ny, fy);
    axypz(nv, C, area, nz, fz);

    return CO_OK;
#   undef A
#   undef S
}
