#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/strain/3d.h>
#include <co/strain/2d.h>

#include <co/dih.h>
#include <co/ddih.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;

int vec(/**/ real a[3]) { vec_argv(&argv, a); return CO_OK; }

static real ks = 0, ka = 3;
static real sq(real x) { return x*x; }

static real  F(__UNUSED void *param, real I1, __UNUSED real I2)          { return I1; }
static real F1(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 1;  }
static real F2(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 0;  }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    enum {X, Y};
    const char *op;
    real a[3], b[3], c[3];
    real a0[3], b0[3], c0[3];
    real da[3], db[3], dc[3];
    real bx, by, cx, cy, ux, uy, wx, wy;
    real I1, I2, area, eng, deng;
    real dvx, dvy, dux, duy, dwx, dwy;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "2d")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_force_3d(NULL, F, F1, F2, a0, b0, c0,   a, b, c,   da, db, dc);
        //vec_printf(da, "%.16g"); vec_printf(db, "%.16g"); vec_printf(dc, "%.16g");
        tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
        tri_3to2(a, b, c,    /**/ &ux, &uy, &wx, &wy);
        strain_2d(NULL, F1, F2,
                           0, 0, bx, by, cx, cy,
                           0, 0, ux - bx, uy - by, wx - cx, wy - cy,
                           &dvx, &dvy, &dux, &duy, &dwx, &dwy,
                           &I1, &I2, &area);
        puts("x y fx fy");
        printf("%.16g %.16g %.16g %.16g\n", 0.0, 0.0, dvx*area, dvy*area);
        printf("%.16g %.16g %.16g %.16g\n",  ux,  uy, dux*area, duy*area);
        printf("%.16g %.16g %.16g %.16g\n",  wx,  wy, dwx*area, dwy*area);
    } else if (eq(op, "3d")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_force_3d(NULL, F, F1, F2, a0, b0, c0,   a, b, c,   da, db, dc);
        puts("x y fx fy");
        printf("%.16g %.16g %.16g %.16g\n", a[X], a[Y], da[X], da[Y]);
        printf("%.16g %.16g %.16g %.16g\n", b[X], b[Y], db[X], db[Y]);
        printf("%.16g %.16g %.16g %.16g\n", c[X], c[Y], dc[X], dc[Y]);
    } else if (eq(op, "energy")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_energy_3d(NULL, F, a0, b0, c0,   a, b, c,   &eng, &deng);
        tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
        tri_3to2(a, b, c,    /**/ &ux, &uy, &wx, &wy);
        strain_2d(NULL, F1, F2,
                           0, 0, bx, by, cx, cy,
                           0, 0, ux - bx, uy - by, wx - cx, wy - cy,
                           NULL, NULL, NULL, NULL, NULL, NULL,
                           &I1, &I2, &area);
        printf("%.16g\n", eng);
    } else if (eq(op, "denergy")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_energy_3d(NULL, F, a0, b0, c0,   a, b, c,   &eng, &deng);
        printf("%.16g\n", deng);
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
