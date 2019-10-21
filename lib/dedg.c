#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/vec.h"

#include "co/dedg.h"

int
dedg_abs(const real a[3], const real b[3], /**/ real da[3], real db[3])
{
    real u[3], e[3];

    vec_minus(b, a, u);
    vec_norm(u, e);
    vec_copy(e, db);
    vec_negative(e, da);
    return CO_OK;
}

int
dedg_sq(const real a[3], const real b[3], /**/ real da[3], real db[3])
{
    real u[3];

    vec_minus(b, a, u);
    vec_scalar(u, 2, db);
    vec_scalar(u, -2, da);
    return CO_OK;
}
