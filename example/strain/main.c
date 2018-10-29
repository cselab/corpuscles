#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/vec.h>
#include <he/macro.h>
#include <he/util.h>
#include <he/strain.h>

#include <he/dih.h>
#include <he/ddih.h>

#define FMT_IN   XE_REAL_IN
static const char **argv;

Strain *strain;
StrainParam param;

int vec(/**/ real a[3]) { vec_argv(&argv, a); return HE_OK; }
int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real a[3], b[3], c[3];
    real a0[3], b0[3], c0[3];
    real da[3], db[3], dc[3];
    real eng;
    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    param.Ks = 0;
    param.Ka = 3;
    strain_ini("skalak", param, /**/ &strain);

    op = *argv++;
    if (eq(op, "force")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_force(strain, a0, b0, c0,   a, b, c,   da, db, dc);
        vec_printf(da, "%g");
        vec_printf(db, "%g");
        vec_printf(dc, "%g");
    } else if (eq(op, "energy")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        eng = strain_energy(strain, a0, b0, c0,   a, b, c);
        printf("%g\n", eng);
    } else
        ER("unknown operation '%s'", op);

    strain_fin(strain);
    return 0;
}
