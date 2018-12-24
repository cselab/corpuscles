include(`he.m4')dnl
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>

#include <he/err.h>
#include <he/he.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/macro.h>
#include <he/off.h>
#include <he/transform.h>
#include <he/util.h>
#include <he/y.h>

#define FMT_OUT HE_REAL_OUT
#define FMT_IN HE_REAL_IN

static const char **argv;
static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
	ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static int eq(const char *a, const char *b) { return util_eq(a, b); }

int main(__UNUSED int argc, const char **v) {
    real *x, *y, *z, com[3], rad;
    int n;
    He *he;

    const char *op;
    argv = v;
    argv++;
    if (*argv == NULL)
	ER("mssing OP");
    op = *argv++;
    y_ini("/dev/stdin", &he, &x, &y, &z);
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

    } else
	ER("unknown operation '%s'", op);
    y_fin(he, x, y, z);
    return HE_OK;
}
