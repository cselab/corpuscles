#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/linear_strain_tri.h>

#include <he/dih.h>
#include <he/ddih.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

enum {X, Y, Z};

int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}

int vec2(/**/ real v[2]) {
    scl(&v[X]);
    scl(&v[Y]);
    return HE_OK;
}

real F1(__UNUSED void *param, real a, real b) { return a*a + b; }
real F2(__UNUSED void *param, real a, real b) { return a*a + b; }


int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real a[2], b[2], c[2];
    real v[2], u[2], w[2];
    real da[2], db[2], dc[2];
    real I1, I2;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "all")) {
        vec2(a); vec2(b); vec2(c);
        vec2(u); vec2(v); vec2(w);
        linear_strain_tri(NULL, F1, F2,
                          a[X], a[Y], b[X], b[Y], c[X], c[Y],
                          v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                          &da[X], &da[Y], &db[X], &db[Y], &dc[X], &dc[Y],
                          &I1, &I2);
        printf("%g %g\n", I1, I2);
        printf("%.8g %.g\n", da[X], da[Y]);        
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
