#include "real.h"

#include "he/err.h"

int constant_strain_force(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
                          const real a0[3], const real b0[3], const real c0[3],
                          const real a[3], const real b[3], const real c[3], /**/
                          real da[3], real db[3], real dc[3]) {

    return HE_OK;
}
