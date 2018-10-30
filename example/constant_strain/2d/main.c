#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/constant_strain/2d.h>

#include <he/dih.h>
#include <he/ddih.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;
static const real h = 1e-6;
static real a[2], b[2], c[2];
static real v[2], u[2], w[2];

enum {X, Y, Z};

static real  F(__UNUSED void *param, real I1, real I2) { return I1; }
static real F1(__UNUSED void *param, real I1, real I2) { return 1;  }
static real F2(__UNUSED void *param, real I1, real I2) { return 0;  }

static int print2(const real a[2], const real b[2]) {
    enum {X, Y};
    printf("%.16g %.16g %.16g %.16g\n", a[X], a[Y], b[X], b[Y]);
    return HE_OK;
}

static real energy() {
    real I1, I2, A;
    constant_strain_2d(NULL, F1, F2,
                       a[X], a[Y], b[X], b[Y], c[X], c[Y],
                       v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                       NULL, NULL, NULL, NULL, NULL, NULL,
                       &I1, &I2, &A);
    return F(NULL, I1, I2);
}

static int scl(/**/ real *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}

static int vec2(/**/ real v[2]) {
    scl(&v[X]);
    scl(&v[Y]);
    return HE_OK;
}

static real fd0(real *p) {
    real e, eh, v, t;
    v = *p;
    e  = energy();
    *p += h;
    eh  = energy();
    return (eh - e)/h;
}

static int fd(real dv[2], real du[2], real dw[2]) {
    enum {X, Y};
    dv[X] = fd0(&v[X]); dv[Y] = fd0(&v[Y]);
    du[X] = fd0(&u[X]); du[Y] = fd0(&u[Y]);
    dw[X] = fd0(&w[X]); dw[Y] = fd0(&w[Y]);
    return HE_OK;
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real da[2], db[2], dc[2];
    real ha[2], hb[2], hc[2];
    real I1, I2, A;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "all")) {
        vec2(a); vec2(b); vec2(c);
        vec2(u); vec2(v); vec2(w);
        constant_strain_2d(NULL, F1, F2,
                           a[X], a[Y], b[X], b[Y], c[X], c[Y],
                           v[X], v[Y], u[X], u[Y], w[X], w[Y], /**/
                           &da[X], &da[Y], &db[X], &db[Y], &dc[X], &dc[Y],
                           NULL, NULL, NULL);
        fd(ha, hb, hc);
        puts("x y fx fy");
        print2(da, ha);
        print2(db, hb);
        print2(dc, hc);
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
