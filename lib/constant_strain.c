#include <stddef.h>

#include "real.h"
#include "he/err.h"

#include "he/memory.h"
#include "he/macro.h"
#include "he/constant_strain.h"

#define T ConstantStrain
#define P ConstantStrainParam

typedef real (*TypeFun)(void*, real, real);

struct T {
    TypeFun F, F1, F2;
    P param;
};

static real sq(real x) { return x*x; }
static real F_skalak(void *p0, real I1, real I2)  {
    P *p;
    real Ks, Ka;
    p = (P*)p0;
    Ks = p->Ks; Ka = p->Ka;

    return (((-2*I2)+sq(I1)+2*I1)*Ks)/12+(sq(I2)*Ka)/12;
}
static real F1_skalak(void *p0, real I1, __UNUSED real I2) {
    P *p;
    real Ks;
    p = (P*)p0;
    Ks = p->Ks;
    return  (I1+1)*Ks/6;
}
static real F2_skalak(void *p0, __UNUSED real I1, real I2) {
    P *p;
    real Ka, Ks;
    p = (P*)p0;
    Ka = p->Ka; Ks = p->Ks;
    return -(Ks-I2*Ka)/6;
}

int constant_strain_ini(__UNUSED const char *name, P param, /**/ T **pq) {
    T *q;
    MALLOC(1, &q);

    q->param = param;
    q->F = F_skalak;
    q->F1 = F1_skalak;
    q->F2 = F2_skalak;

    *pq = q;
    return HE_OK;
}
int constant_strain_fin(T *q) {
    FREE(q);
    return HE_OK;
}

int constant_strain_force(T *q,
                          const real a0[3], const real b0[3], const real c0[3],
                          const real a[3], const real b[3], const real c[3], /**/
                          real da[3], real db[3], real dc[3]) {
    return HE_OK;
}

real constant_strain_energy(T* q,
                            const real a0[3], const real b0[3], const real c0[3],
                            const real a[3], const real b[3], const real c[3]) {
    return 0.0;
}
