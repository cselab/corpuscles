#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/ring.h>
#include <co/util.h>
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
    real *x, *y, *z;
    He      *he;
    int nv, i, n, *rring, status;
    Ring *ring;
    real *out, *xyz, *C, xu[3], xv[3];

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

    for (i = 0; i < nv; i++) {
        if (he_bnd_ver(he, i)) continue;
        status = he_ring(he, i, &n, &rring);
        if (status != CO_OK)
            ER("he_ring failed for i = %d", i);
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
            ring_xyz(ring, i, rring, x, y, z, &out);
            printf("%d\n", n + 1);
            write(3, n + 1, out);
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
            ring_xyz(ring, i, rring, x, y, z, &xyz);
            ring_C(ring, i, rring, x, y, z, &C);
            ring_xu(n, xyz, C, xu);
            vec_printf(xu, FMT);
        } else if (eq(op, "xv")) {
            ring_xyz(ring, i, rring, x, y, z, &xyz);
            ring_C(ring, i, rring, x, y, z, &C);
            ring_xv(n, xyz, C, xv);
            vec_printf(xv, FMT);
        } else
            ER("unknown operation '%s'", op);
    }
    ring_fin(ring);
    return y_fin(he, x, y, z);
}
