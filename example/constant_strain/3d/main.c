#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/constant_strain/3d.h>

#include <he/dih.h>
#include <he/ddih.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

int vec(/**/ real a[3]) { vec_argv(&argv, a); return HE_OK; }

static real ks = 0, ka = 3;
static real sq(real x) { return x*x; }
static real  F(__UNUSED void *param, real I1, real I2) { return (((-2*I2)+sq(I1)+2*I1)*ks)/12+(sq(I2)*ka)/12; }
static real F1(__UNUSED void *param, real I1, real I2) { return  (I1+1)*ks/6; }
static real F2(__UNUSED void *param, real I1, real I2) { return -(ks-I2*ka)/6;}

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real a[3], b[3], c[3];
    real a0[3], b0[3], c0[3];
    real da[3], db[3], dc[3];
    real eng, deng;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "force")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        constant_strain_force(
            NULL, F, F1, F2,
            a0, b0, c0,   a, b, c,   da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "energy")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        constant_strain_energy(NULL, F, a0, b0, c0,   a, b, c,   &eng, &deng);
        printf("%g %g\n", eng, deng);
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
