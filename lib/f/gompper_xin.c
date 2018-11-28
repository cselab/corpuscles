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
#include "he/macro.h"

#include "he/f/gompper_xin.h"

#define T HeFGompperXin

#define BEGIN_VER                                \
    nv = he_nv(he);                              \
    for (i = 0; i < nv; i++) {
#define END_VER }

struct T {
    real Kb, H0;
    real *energy;
};

int he_f_gompper_xin_ini(real Kb, real C0, __UNUSED real Kad, __UNUSED real DA0D, He *he, T **pq) {
    T *q;
    int nv;

    MALLOC(1, &q);
    nv = he_nv(he);
    CALLOC(nv, &q->energy);

    q->Kb = Kb;
    q->H0 = C0/2;

    *pq = q;
    return HE_OK;
}

int he_f_gompper_xin_fin(T *q) {
    FREE(q->energy);
    FREE(q);
    return HE_OK;
}

int he_f_gompper_xin_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *fx_tot, real *fy_tot, real *fz_tot) {
    return HE_OK;
}

real he_f_gompper_xin_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
#   define A(f) f = q->f
    int nv, i;
    real *energy;
    real Kb, H0;

    A(energy); A(Kb); A(H0);

    nv = he_nv(he);
    BEGIN_VER {
        energy[i] = 0;
    } END_VER;

    return he_sum_array(nv, energy);
#   undef A
}

int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
