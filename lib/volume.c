#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/sum.h"

#include "he/volume.h"

static void get(int t, He *he,
                const real *x, const real *y, const real *z,
                /**/ real a[3], real b[3], real c[3]) {
    int h;
    int i, j, k;
    h = he_hdg_tri(he, t);
    he_ijk(he, h, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}

real he_volume_tri(He *he, const real *x, const real *y, const real *z) {
    int n, m;
    real v, V;
    real a[3], b[3], c[3];
    HeSum *sum;

    n = he_nt(he);
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b, c);
        v = tri_volume(a, b, c);
        he_sum_add(sum, V);
    }
    V = he_sum_get(sum);
    he_sum_fin(sum);
    return V;
}
