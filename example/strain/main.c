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
static const real h = 1e-6;

static Strain *strain;
static StrainParam param;

static real a[3], b[3], c[3];
static real a0[3], b0[3], c0[3];

real fd0(real *p) {
    real e, eh, v, t;
    v = *p;
    e  = strain_energy(strain, a0, b0, c0,   a, b, c);
    *p += h;
    eh  = strain_energy(strain, a0, b0, c0,  a, b, c);
    *p = v;
    return (eh - e)/h;
}

int fd(real da[3], real db[3], real dc[3]) {
    enum {X, Y, Z};
    da[X] = fd0(&a[X]); da[Y] = fd0(&a[Y]); da[Z] = fd0(&a[Z]);
    db[X] = fd0(&b[X]); db[Y] = fd0(&b[Y]); db[Z] = fd0(&b[Z]);
    dc[X] = fd0(&c[X]); dc[Y] = fd0(&c[Y]); dc[Z] = fd0(&c[Z]);
    return HE_OK;
}

int print3(const real a[3], const real b[3], const real c[3]) {
    enum {X, Y, Z};
    printf("%.16g %.16g %.16g %.16g %.16g %.16g %.16g %.16g %.16g\n",
           a[X], a[Y], a[Z], b[X], b[Y], b[Z], c[X], c[Y], c[Z]);
    return HE_OK;
}

int vec(/**/ real a[3]) { vec_argv(&argv, a); return HE_OK; }
int eq(const char *a, const char *b) { return util_eq(a, b); }
int main(__UNUSED int argc, const char **argv0) {
    const char *op;
    real eng;
    real da[3], db[3], dc[3];
    real ha[3], hb[3], hc[3];    

    argv = argv0;
    argv++;
    if (*argv == NULL) ER("mssing OP");

    param.Ks = 1;
    param.Ka = 0;
    strain_ini("bug", param, /**/ &strain);

    op = *argv++;
    if (eq(op, "force")) {
        vec(a0); vec(b0); vec(c0);
        vec(a); vec(b); vec(c);
        eng = strain_energy(strain, a0, b0, c0,   a, b, c);
        MSG("eng: %g", eng);
        strain_force(strain, a0, b0, c0,   a, b, c,   da, db, dc);
        fd(ha, hb, hc);
        printf("x y z fx fy fz hx hy hz\n");
        print3(a, da, ha);
        print3(b, db, hb);
        print3(c, dc, hc);

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
