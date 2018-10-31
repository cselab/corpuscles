#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/tri.h>
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
    real a[3], b[3], c[3], d[3], n[3], f[3];
    real ux, uy, vx, vy, fx, fy;
    argv = v;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "angle")) {
        vec(a); vec(b); vec(c);
        printf("%.16g\n", tri_angle(a, b, c));
    } else if (eq(op, "area")) {
        vec(a); vec(b); vec(c);
        printf("%.16g\n", tri_area(a, b, c));
    } else if (eq(op, "cot")) {
        vec(a); vec(b); vec(c);
        printf("%.16g\n", tri_cot(a, b, c));
    } else if (eq(op, "volume")) {
        vec(a); vec(b); vec(c);
        printf("%.16g\n", tri_volume(a, b, c));
    } else if (eq(op, "normal")) {
        vec(a); vec(b); vec(c);
        tri_normal(a, b, c, /**/ n);
        vec_printf(n, "%g");
    } else if (eq(op, "3to2")) {
        vec(a); vec(b); vec(c);
        tri_3to2(a, b, c, /**/ &ux, &uy, &vx, &vy);
        printf("%.16g %.16g\n", ux, uy);
        printf("%.16g %.16g\n", vx, vy);
    } else if (eq(op, "2to3")) {
        vec(a); vec(b); vec(c); scl(&fx); scl(&fy);
        tri_2to3(a, b, c, fx, fy, /**/ f);
        vec_printf(f, "%.16g");
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
