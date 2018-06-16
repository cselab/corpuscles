#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"

#include "he/edg.h"

real edg_abs(const real a[3], const real b[3]) {
    real u[3];
    vec_minus(b, a, u);
    return vec_abs(u);
}
