#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/macro.h>
#include <co/util.h>
#include <co/strain.h>

#include <co/dih.h>
#include <co/ddih.h>

#define FMT_IN   CO_REAL_IN
static const char **argv;
static const real h = 1e-6;

static Strain *strain;
static StrainParam param;

static real a[3], b[3], c[3];
static real a0[3], b0[3], c0[3];

int print3(const real a[3], const real b[3], const real c[3]) {
    enum {X, Y, Z};
    printf("%.16g %.16g %.16g %.16g %.16g %.16g %.16g %.16g %.16g\n",
           a[X], a[Y], a[Z], b[X], b[Y], b[Z], c[X], c[Y], c[Z]);
    return CO_OK;
}

int vec(/**/ real a[3]) { vec_argv(&argv, a); return CO_OK; }
int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real eng;
    real da[3], db[3], dc[3];

    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    param.Ka = 0;
    param.mu = 2;
    param.a3 = param.a3 = 0;
    param.b1 = param.b2 = 0;
    strain_ini("lim", param, /**/ &strain);

    op = *argv++;
    if (eq(op, "force")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        eng = strain_energy(strain, a0, b0, c0,   a, b, c);
        MSG("eng: %g", eng);
        strain_force(strain, a0, b0, c0,   a, b, c,   da, db, dc);
        puts("x0 y0 z0 x y z fx fy fz");
        print3(a0, a, da);
        print3(b0, b, db);
        print3(c0, c, dc);
    } else if (eq(op, "energy")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        eng = strain_energy(strain, a0, b0, c0,   a, b, c);
        printf("%g\n", eng);
    } else if (eq(op, "off")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        strain_force(strain, a0, b0, c0,   a, b, c,   da, db, dc);
        tri_list(a, b, c, da, db, dc, stdout);
    } else
        ER("unknown operation '%s'", op);
    strain_fin(strain);
    return 0;
}
