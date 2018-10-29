#include <stddef.h>

#include "real.h"
#include "he/err.h"

#include "he/memory.h"
#include "he/constant_strain.h"

#define T ConstantStrain
#define P ConstantStrainParam

int constant_strain_ini(const char *name, P param, /**/ T **pq) {
    return HE_OK;
}
int constant_strain_fin(T *q) {
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
