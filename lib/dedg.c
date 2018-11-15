#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"

#include "he/dedg.h"

int dedg_abs(const real a[3], const real b[3], /**/ real da[3], real db[3]) {
    real u[3], e[3];
    vec_minus(b, a,   u);
    vec_norm(u,   e);
    vec_copy(e,     db);
    vec_negative(e, da);
    return HE_OK;
}

int dedg_sq(const real a[3], const real b[3], /**/ real da[3], real db[3]) {
    real u[3];
    vec_minus(b, a,   u);
    vec_scalar(u,  2,    db);
    vec_scalar(u, -2,    da);
    return HE_OK;
}
