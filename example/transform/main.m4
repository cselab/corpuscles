include(`co.m4')dnl
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>

#include <co/err.h>
#include <co/he.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/macro.h>
#include <co/off.h>
#include <co/transform.h>
#include <co/util.h>
#include <co/y.h>

#define FMT_OUT CO_REAL_OUT
#define FMT_IN CO_REAL_IN

static const char **argv;
static void vec(/**/ real a[3]) { vec_argv(&argv, a); }
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
	ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static int eq(const char *a, const char *b) { return util_eq(a, b); }

int main(__UNUSED int argc, const char **v) {
    real *x, *y, *z, r[3], com[3], rad;
    int n;
    He *he;

    const char *op;
    argv = v;
    argv++;
    if (*argv == NULL)
	ER("mssing OP");
    op = *argv++;
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);

    if (eq(op, "centroid")) {
	transform_centroid(he, x, y, z, /**/ com);
	vec_printf(com, FMT_OUT);

    h_define(`OP', `
    } else if (eq(op, "$1")) {
	scl(&rad);
	transform_$1(rad, n, /*io*/ x, y, z);
	off_he_xyz_fwrite(he, x, y, z, stdout);
    ')dnl
    OP(`rotx')dnl
    OP(`roty')dnl
    OP(`rotz')dnl
    OP(`tranx')dnl
    OP(`trany')dnl
    OP(`tranz')dnl
    OP(`scalx')dnl
    OP(`scaly')dnl
    OP(`scalz')dnl

    h_define(`OP3', `
    } else if (eq(op, "$1")) {
        vec(r);
	transform_$1(r, n, /*io*/ x, y, z);
	off_he_xyz_fwrite(he, x, y, z, stdout);
    ')dnl
    OP3(tran)dnl
    OP3(scal)dnl

    } else
	ER("unknown operation '%s'", op);
    y_fin(he, x, y, z);
    return CO_OK;
}
