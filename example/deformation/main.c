#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/strain/3d.h>

#include <he/dih.h>
#include <he/ddih.h>

#define FMT_IN   HE_REAL_IN
static const char **argv;

int vec(/**/ real a[3]) { vec_argv(&argv, a); return HE_OK; }

static real  F_I1(__UNUSED void *param, real I1, __UNUSED real I2) { return I1; }
static real  F_I2(__UNUSED void *param, __UNUSED  real I1, real I2) { return I2; }

int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    enum {X, Y};
    const char *op;
    real a0[3], b0[3], c0[3];    
    real a[3], b[3], c[3];
    real eng, deng;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    op = *argv++;
    if (eq(op, "I1")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_energy_3d(NULL, F_I1, a0, b0, c0,   a, b, c,   &eng, &deng);
        printf("%.16g\n", eng);
    } else
        ER("unknown operation '%s'", op);
    return 0;
}
