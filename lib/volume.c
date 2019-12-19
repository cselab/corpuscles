#include <math.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/sum.h"

#include "co/volume.h"

static int
get(int t, He * he,
    const real * x, const real * y, const real * z,
    /**/ real a[3], real b[3], real c[3])
{
    int i, j, k;

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

real
he_volume_tri(He * he, const real * x, const real * y, const real * z)
{
    int n, m;
    real v, V;
    real a[3], b[3], c[3];
    HeSum *sum;

    sum_ini(&sum);
    n = he_nt(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b, c);
        v = tri_volume(a, b, c);
        sum_add(sum, v);
    }
    V = sum_get(sum);
    sum_fin(sum);
    return V;
}
