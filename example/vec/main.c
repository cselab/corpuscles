#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/macro.h>
#include <co/util.h>

#define FMT_IN   CO_REAL_IN
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
    const char *op;
    real a[3], b[3], c[3], m[3], s;
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "cross")) {
        vec(a); vec(b);
        vec_cross(a, b, c);
        vec_printf(c, "%g");
    } else if (eq(op, "norm")) {
        vec(a);
        vec_norm(a, b);
        vec_printf(b, "%g");
    } else if (eq(op, "abs")) {
        vec(a);
        s = vec_abs(a);
        printf("%g\n", s);
    } else if (eq(op, "cylindrical_r")) {
        vec(a);
        s = vec_cylindrical_r(a);
        printf("%g\n", s);
    } else if (eq(op, "mean3")) {
        vec(a); vec(b); vec(c);
        vec_mean3(a, b, c, /**/ m);
        vec_printf(m, "%g");
    } else if (eq(op, "add")) {
        vec(a); vec(b);
        vec_add(a, b);
        vec_printf(b, "%g");
    } else if (eq(op, "axpy")) {
        scl(&s); vec(a); vec(b);
        vec_axpy(s, a, b);
        vec_printf(b, "%g");
    } else if (eq(op, "neg")) {
        vec(a);
        vec_neg(a);
        vec_printf(a, "%g");
    } else if (eq(op, "scale")) {
        scl(&s);
        vec(a);
        vec_scale(s, a);
        vec_printf(a, "%g");
    } else if (eq(op, "project")) {
        vec(a); vec(b);
        vec_project(a, b, c);
        vec_printf(c, "%g");
    } else if (eq(op, "reject")) {
        vec(a); vec(b);
        vec_reject(a, b, c);
        vec_printf(c, "%g");
    } else if (eq(op, "project_scalar")) {
        vec(a); vec(b);
        s = vec_project_scalar(a, b);
        printf("%g\n", s);
    } else if (eq(op, "reject_scalar")) {
        vec(a); vec(b);
        s = vec_reject_scalar(a, b);
        printf("%g\n", s);
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
