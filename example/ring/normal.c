#include <stdio.h>
#include <stdlib.h>

#include <real.h>

#include <co/array.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ring.h>
#include <co/util.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

int main(__UNUSED int argc, __UNUSED const char **argv) {
    real *x, *y, *z, *scalar;
    He      *he;
    int nv, i, n, *rring, status;
    Ring *ring;
    real *xyz, *C, u[3], r[3];

    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    ring_ini(&ring);
    nv = he_nv(he);
    MALLOC(nv, &scalar);
    array_copy(nv, z, scalar);

    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i)) continue;
        status = he_ring(he, i, &n, &rring);
        if (status != CO_OK)
            ER("he_ring failed for i = %d", i);
        ring_xyz(ring, i, rring, x, y, z, &xyz);
        ring_C(ring, i, rring, x, y, z, &C);
	ring_normal(n, xyz, C, u);
	vec_get(i, x, y, z, r);
	vec_printf(r, FMT);
	vec_printf(u, FMT);
    }

    FREE(scalar);
    ring_fin(ring);
    return y_fin(he, x, y, z);
}
