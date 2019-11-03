#include <stdio.h>

#include "real.h"

#include "co/err.h"
#include "co/fd2.h"
#include "co/force2.h"
#include "co/skel.h"

static real
fd0(real * p, Force2 * f, Skel * skel, real delta, real * x, real * y)
{
    real t, hi, lo;

    t = *p;
    *p += delta;
    hi = force2_energy(f, skel, x, y);
    *p = t;
    *p -= delta;
    lo = force2_energy(f, skel, x, y);
    *p = t;
    return (hi - lo) / (2 * delta);
}

int
fd2(Force2 * f, Skel * skel, real delta, real * x, real * y, real * fx,
    real * fy)
{
    int n, i;

    n = skel_nv(skel);
    for (i = 0; i < n; i++) {
        fx[i] = fd0(&x[i], f, skel, delta, x, y);
        fy[i] = fd0(&y[i], f, skel, delta, x, y);
    }
    return CO_OK;
}
