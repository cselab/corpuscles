#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>
#include <co/bending.h>
#include <co/punto.h>
#include <co/he.h>
#include <co/off.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/util.h>
#include <co/macro.h>
#include <co/vec.h>
#include <co/vtk.h>
#define FMT_IN   CO_REAL_IN

static const char **argv;
static char name[4048];


static real Kb, C0, Kad, DA0D;
static real h;
static int  every;
static real *xx, *yy, *zz;
static real *eng;
static real *Fx, *Fy, *Fz, *Fm;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "fd";

static void usg(void) {
    fprintf(stderr, "%s kantor/gompper/gompper_kroll/juelicher/juelicher_xin/meyer/meyer_xin Kb C0 Kad DA0D step  every < OFF > forces\n", me);
}

static int eq(const char *a, const char *b) { return util_eq(a, b); }
static int scl(/**/ real *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*argv, FMT_IN, p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static int num(/**/ int *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*argv, "%d", p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}
static int arg() {
    if (*argv != NULL && eq(*argv, "-h")) {
        usg();
        exit(0);
    }
    str(name);
    scl(&param.Kb); scl(&param.C0); scl(&param.Kad); scl(&param.DA0D);
    scl(&h);
    num(&every);
    if (every < 1)
      ER("every < 1");
    return CO_OK;
}

static int diff(int id, /**/ real f[3]) {
    real tmp, e, eh;
    enum {X, Y, Z};
    e = bending_energy(bending, he, xx, yy, zz);
    tmp = xx[id]; xx[id] += h; eh = bending_energy(bending, he, xx, yy, zz); f[X] = (eh - e)/h; xx[id] = tmp;
    tmp = yy[id]; yy[id] += h; eh = bending_energy(bending, he, xx, yy, zz); f[Y] = (eh - e)/h; yy[id] = tmp;
    tmp = zz[id]; zz[id] += h; eh = bending_energy(bending, he, xx, yy, zz); f[Z] = (eh - e)/h; zz[id] = tmp;
    return CO_OK;
}

static void main0() {
    enum {X, Y, Z};
    int i;
    real e, f[3];
    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ Fx, Fy, Fz);

    e = bending_energy(bending, he, xx, yy, zz);
    bending_energy_ver(bending, /**/ &eng);

    MSG("energy: %g", e);
    for (i = 0; i < nv; i += every) {
        diff(i, /**/ f);
        printf("%g %g\n", f[X], Fx[i]);
        Fm[i] = Fx[i] - f[X];
    }

    if (every == 1) {
        const real *scalars[] = {Fx, Fy, Fz, Fm, NULL};
        const char *names[]   = {"fx", "fy", "fz", "fm", NULL};
        vtk_write(he, xx, yy, zz, scalars, names, "q.vtk");
    }

    bending_fin(bending);
}

int main(int __UNUSED argc, const char *v[]) {
    int *tri;
    const char path[] = "/dev/stdin";
    static HeOff *off;
    argv = v; argv++;
    arg();

    off_ini(path, &off);

    nv = off_nv(off);
    nt = off_nt(off);
    off_tri(off, &tri);
    he_tri_ini(nv, nt, tri, &he);

    MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
    CALLOC(nv, &Fm); CALLOC(nv, &Fx);  CALLOC(nv, &Fy); CALLOC(nv, &Fz);

    off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(Fm); FREE(Fx); FREE(Fy); FREE(Fz);

    off_fin(off);
    he_fin(he);
}
