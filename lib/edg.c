#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/vec.h"

#include "co/edg.h"

real edg_abs(const real a[3], const real b[3]) {
    real u[3];
    vec_minus(b, a, u);
    return vec_abs(u);
}

real edg_sq(const real a[3], const real b[3]) {
    real u[3];
    vec_minus(b, a, u);
    return vec_dot(u, u);
}
