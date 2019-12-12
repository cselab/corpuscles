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

static const char **argv;

static const char *me = "ring";
static void usg(void) {
    fprintf(stderr, "%s OP [ARG..]\n", me);
    exit(2);
}

static int write(int n, int m, real *A) {
    int i, j, s;
    for (i = s = 0; i < n; i++) {
	for (j = 0; j < m; j++, s++) {
	    if (j > 0) printf(" ");
	    printf(FMT, A[s]);
	}
	printf("\n");
    }
    return CO_OK;
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v0) {
    const char *op;
    real *x, *y, *z, *scalar;
    He      *he;
    int nv, i, n, *rring, status;
    Ring *ring;
    real *out, *xyz, *C, *wgrad, *scalar_grid, xx[3], gcov[2*2], gcnt[2*2], u[3];

    argv = v0;
    argv++;
    if (*argv == NULL)
	ER("mssing OP");
    op = *argv++;
    if (eq(op, "-h"))
	usg();

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
	if (eq(op, "alpha")) {
	    ring_alpha(ring, i, rring, x, y, z, &out);
	    write(1, n, out);
	} else if (eq(op, "beta")) {
	    ring_beta(ring, i, rring, x, y, z, &out);
	    write(1, n, out);
	} else if (eq(op, "theta")) {
	    ring_theta(ring, i, rring, x, y, z, &out);
	    write(1, n, out);
	} else if (eq(op, "xyz")) {
	    printf("%d\n", n + 1);
	    write(3, n + 1, xyz);
	} else if (eq(op, "A")) {
	    ring_A(ring, i, rring, x, y, z, &out);
	    printf("%d\n", n + 1);
	    write(n + 1, 6, out);
	} else if (eq(op, "B")) {
	    ring_B(ring, i, rring, x, y, z, &out);
	    write(6, 6, out);
	} else if (eq(op, "C")) {
	    ring_C(ring, i, rring, x, y, z, &out);
	    write(6, n + 1, out);
	} else if (eq(op, "xu")) {
	    ring_xu(n, xyz, C, xx);
	    vec_printf(xx, FMT);
	} else if (eq(op, "xv")) {
	    ring_xv(n, xyz, C, xx);
	    vec_printf(xx, FMT);
	} else if (eq(op, "xuu")) {
	    ring_xuu(n, xyz, C, xx);
	    vec_printf(xx, FMT);
	} else if (eq(op, "xuv")) {
	    ring_xuv(n, xyz, C, xx);
	    vec_printf(xx, FMT);
	} else if (eq(op, "xvv")) {
	    ring_xvv(n, xyz, C, xx);
	    vec_printf(xx, FMT);
	} else if (eq(op, "gcov")) {
	    ring_gcov(n, xyz, C, gcov);
	    write(1, 3, gcov);
	} else if (eq(op, "gcnt")) {
	    ring_gcnt(n, xyz, C, gcnt);
	    write(1, 3, gcnt);
	} else if (eq(op, "g")) {
	    printf(FMT "\n", ring_g(n, xyz, C));
	} else if (eq(op, "normal")) {
	    ring_normal(n, xyz, C, u);
	    vec_printf(u, FMT);
	} else if (eq(op, "wgrad")) {
	    ring_wgrad(ring, n, xyz, C, &out);
	    matrix_fwrite(3, n + 1, out, stdout);
	} else if (eq(op, "scalar")) {
	    ring_scalar(ring, i, rring, scalar, &out);
	    matrix_fwrite(1, n + 1, out, stdout);
	} else if (eq(op, "grad")) {
	    ring_scalar(ring, i, rring, scalar, &scalar_grid);
	    ring_wgrad(ring, n, xyz, C, &wgrad);
	    printf(FMT "\n", ring_grad(n, wgrad, scalar_grid));
	} else if (eq(op, "H")) {
	    printf(FMT "\n", ring_H(n, xyz, C));
	} else if (eq(op, "K")) {
	    printf(FMT "\n", ring_K(n, xyz, C));
	} else
	    ER("unknown operation '%s'", op);
    }

    FREE(scalar);
    ring_fin(ring);
    return y_fin(he, x, y, z);
}
