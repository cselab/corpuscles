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
#include "he/volume.h"

#include "he/f/volume_normal.h"

#define T HeFVolumeNormal

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

struct T {
    real v0, K;
    Dh *dh;

    int nv;
    real *H;
};

static int axypz(int n, real a, const real *x, const real *y,
                 /**/ real *z) {
    int i;
    for (i = 0; i < n; i++)
        z[i] += a*x[i]*y[i];
    return HE_OK;
}

int he_f_volume_normal_ini(real v0, real K, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    M(nv, H);

    S(nv);
    S(v0); S(K);

    dh_ini(he, &q->dh);

    *pq = q;
    return HE_OK;
#   undef S
#   undef M
}

int he_f_volume_normal_argv(char ***p, He *he, T **pq) {
    return HE_OK;
}

int he_f_volume_normal_fin(T *q) {
#   define F(x) FREE(q->x)
    dh_fin(q->dh);
    F(H);
    FREE(q);
    return HE_OK;
#   undef F
}

real he_f_volume_normal_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
    real v0, K;
    real C, V, d;

    G(v0); G(K);

    V = he_volume_tri(he, x, y, z);
    d = V - v0;
    C = K/v0;
    return C*d*d;
#   undef A
#   undef S
}

int he_f_volume_normal_force(T *q, He *he,
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

    V = he_volume_tri(he, x, y, z);
    d = V - v0;
    C = -2*(K/v0)*d; /* TODO: invert nomral */
    axypz(nv, C, area, nx, fx);
    axypz(nv, C, area, ny, fy);
    axypz(nv, C, area, nz, fz);

    return HE_OK;
#   undef A
#   undef S
}
