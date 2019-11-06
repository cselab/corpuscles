#include <math.h>
#include <stdio.h>

#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/i/vec.h"
#include "co/i/tri.h"

#include "co/normal.h"

static real pi = 3.141592653589793115997964;
int
normal_mwa(He * he, const real * x, const real * y, const real * z, /**/
           real * nx, real * ny, real * nz)
{
    enum { X, Y, Z };
    int *t0, *t1, *t2;
    int t, nt;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, &t0, &t1, &t2);

    array_zero3(nv, nx, ny, nz);
    for (t = 0; t < nt; t++) {
        i = t0[t];
        j = t1[t];
        k = t2[t];
        i_vec_get3(i, j, k, x, y, z, a, b, c);
        theta_a = i_tri_angle(c, a, b);
        theta_b = i_tri_angle(a, b, c);
        theta_c = pi - theta_a - theta_b;
        i_tri_normal(a, b, c, u);
        vec_scalar_append(u, theta_a, i, nx, ny, nz);
        vec_scalar_append(u, theta_b, j, nx, ny, nz);
        vec_scalar_append(u, theta_c, k, nx, ny, nz);
    }

    for (i = 0; i < nv; i++) {
        vec_get(i, nx, ny, nz, /**/ u);
        vec_norm(u, /**/ u0);
        vec_set(u0, i, /**/ nx, ny, nz);
    }
    return CO_OK;
}
