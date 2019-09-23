#include <stdio.h>
#include <stdlib.h>

#include <real.h>

#include <co/area.h>
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
    real *x, *y, *z, *area, r0;
    He      *he;
    int nv, i, n, *rring, status;
    Ring *ring;
    real *xyz, *C, r[3], H, K;

    err_set_ignore();

    y_inif(stdin, &he, &x, &y, &z);
    ring_ini(&ring);
    nv = he_nv(he);
    MALLOC(nv, &area);
    he_area_ver(he, x, y, z, area);
    
    fputs("x y z r area H K\n", stdout);
    for (i = 0; i < nv; i++) {
	if (he_bnd_ver(he, i)) continue;
	status = he_ring(he, i, &n, &rring);
	if (status != CO_OK)
	    ER("he_ring failed for i = %d", i);
	ring_xyz(ring, i, rring, x, y, z, &xyz);
	ring_C(ring, i, rring, x, y, z, &C);
	H = ring_H(n, xyz, C);
	K = ring_K(n, xyz, C);
	vec_get(i, x, y, z, r);
	r0 = vec_cylindrical_r(r);
	vec_fprintf0(r, stdout, FMT);
	fprintf(stdout, " " FMT " " FMT " " FMT " " FMT "\n", r0, area[i], H, K);
    }

    FREE(area);
    ring_fin(ring);
    return y_fin(he, x, y, z);
}
