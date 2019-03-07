#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"

#include "co/normal.h"

static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}
int normal_mwa(He *he, const real *x, const real *y, const real *z, /**/
                  real *normx, real *normy, real *normz) {
    enum {X, Y, Z};
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    array_zero3(nv, normx, normy, normz);
    for ( t = 0; t < nt; t++ ) {
        he_tri_ijk(he, t, &i, &j, &k);
        get3(x, y, z, i, j, k, a, b, c);
        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);
        tri_normal(a, b, c, u);
        vec_scalar_append(u, theta_a, i, normx, normy, normz);
        vec_scalar_append(u, theta_b, j, normx, normy, normz);
        vec_scalar_append(u, theta_c, k, normx, normy, normz);
    }

    for (i = 0; i < nv; i++) {
        vec_get(i, normx, normy, normz, /**/ u);
        vec_norm(u, /**/ u0);
        vec_set(u0, i, /**/ normx, normy, normz);
    }
    return CO_OK;
}
