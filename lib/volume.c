#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"

#include "he/volume.h"

static void get(int t, He *he,
                const real *x, const real *y, const real *z,
                /**/ real a[3], real b[3], real c[3]) {
    int h, n, nn;
    int i, j, k;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    
    i = he_ver(he, h);
    j = he_ver(he, n);
    k = he_ver(he, nn);
    
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}

real he_volume_tri(He *he, const real *x, const real *y, const real *z) {
    int n, m;
    real s;
    real a[3], b[3], c[3];

    n = he_nt(he);
    s = 0;
    for (m = 0; m < n; m++) {
        get(m, he, x, y, z, /**/ a, b, c);
        s += tri_volume(a, b, c);
    }
    return s;
}
