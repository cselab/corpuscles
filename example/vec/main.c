#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

void vec(/**/ real a[3]) { vec_argv(&argv, a); }
int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **v) {
    const char *op;
    real a[3], b[3], c[3], s;
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
