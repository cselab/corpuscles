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
    return 0.0;
}

int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
