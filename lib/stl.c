#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/stl.h"

int
stl_write(He * he, const real * x, const real * y, const real * z,
          /**/ FILE * f)
{
    int np, nv, nt, r, i, n_sc, i_sc;
    int a, b, c;

    np = 3;
    nv = he_nv(he);
    nt = he_nt(he);

    return CO_OK;
}
