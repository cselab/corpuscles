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
#include "he/da.h"
#include "he/macro.h"

#include "he/f/garea_voronoi.h"

#define T HeFGareaVoronoi

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

struct T {
    real A0, K;
    real *fx, *fy, *fz;
    Da *da;

    int nv;
    real *H;
};

static real e(real area0, real area) {
    real d;
    d = area - area0;
    return d*d;
}
static real dda(void *p, real area) {
    real area0, d;
    area0 = *(real*)p;
    d = area - area0;
    return 2*d;
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

int he_f_garea_voronoi_ini(real A0, real K, He *he, T **pq) {
#   define M(n, f) MALLOC(n, &q->f)
#   define S(f) q->f = f
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    M(nv, fx); M(nv, fy); M(nv, fz);
    M(nv, H);

    S(nv);
    S(A0); S(K);

    da_ini(he, &q->da);

    *pq = q;
    return HE_OK;
#   undef S
#   undef M
}

int he_f_garea_voronoi_fin(T *q) {
#   define F(x) FREE(q->x)
    da_fin(q->da);
    F(fx); F(fy); F(fz);
    F(H);
    FREE(q);
    return HE_OK;
#   undef F
}

real he_f_garea_voronoi_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    Da *da;
    real A0, K;
    real A, *area, d;

    G(A0); G(K);
    G(da);

    nv = he_nv(he);

    da_compute_area(da, he, x, y, z);
    da_area(da, &area);

    A = he_sum_array(nv, area);
    d = A - A0;
    return K/A0*d*d;
#   undef A
#   undef S
}

int he_f_garea_voronoi_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *hx, real *hy, real *hz) {
    /* get, set */
#   define G(f) f = q->f
    int nv;
    real A0, K;
    real *fx, *fy, *fz;
    real C;
    Da *da;
    dAParam param;

    G(A0); G(K);
    G(da);
    G(fx); G(fy); G(fz);

    nv = he_nv(he);
    zero(nv, fx); zero(nv, fy); zero(nv, fz);

    param.da = dda;
    param.p  = (void*)&A0;
    da_force(da, param, he, x, y, z, /**/ fx, fy, fz);

    C = K/A0;
    scale(C, nv, fx); scale(C, nv, fy); scale(C, nv, fz);
    plus(nv, fx, hx); plus(nv, fy, hy); plus(nv, fz, hz);

    return HE_OK;
#   undef A
#   undef S
}
