#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>
#include <he/bending.h>
#include <he/punto.h>
#include <he/he.h>
#include <he/off.h>
#include <he/memory.h>
#include <he/err.h>
#include <he/util.h>
#include <he/macro.h>
#include <he/vec.h>
#define FMT_IN   XE_REAL_IN

static const char **argv;
static char name[4048];


static real Kb, C0, Kad, DA0D;
static real h;
static int  every;
static real *xx, *yy, *zz, *rxy;
static real *eng;
static real *Fx, *Fy, *Fz, *Fm;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "fd";

static void usg() {
    fprintf(stderr, "%s kantor/gompper/gompper_kroll/juelicher/meyer Kb C0 Kad DA0D step every < OFF > forces\n", me);
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
    return HE_OK;
}
static int num(/**/ int *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*argv, "%d", p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return HE_OK;
}
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
}
static int arg() {
    if (*argv != NULL && eq(*argv, "-h")) {
        usg();
        exit(0);
    }
    str(name);
    scl(&Kb);
    scl(&C0);
    scl(&Kad);
    scl(&DA0D);
    scl(&h);
    num(&every);
    //printf("%s,%f,%f,%f,%f,%f,%i\n", name, Kb, C0, Kad, DA0D, h, every);
    if (every < 1)
      ER("every < 1");
    return HE_OK;
}

static int diff(int id, /**/ real f[3]) {
    real tmp, e, eh;
    enum {X, Y, Z};
    e = bending_energy(bending, he, xx, yy, zz);
    tmp = xx[id]; xx[id] += h; eh = bending_energy(bending, he, xx, yy, zz); f[X] = (eh - e)/h; xx[id] = tmp;
    tmp = yy[id]; yy[id] += h; eh = bending_energy(bending, he, xx, yy, zz); f[Y] = (eh - e)/h; yy[id] = tmp;
    tmp = zz[id]; zz[id] += h; eh = bending_energy(bending, he, xx, yy, zz); f[Z] = (eh - e)/h; zz[id] = tmp;
    return HE_OK;
}

static void main0() {
    enum {X, Y, Z};
    int i;
    real e, f[3];
    param.Kb = Kb;
    param.C0 = C0;
    param.Kad = Kad;
    param.DA0D = DA0D;

    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ Fx, Fy, Fz);

    e = bending_energy(bending, he, xx, yy, zz);
    bending_energy_ver(bending, /**/ &eng);

    MSG("energy: %g", e);
    for (i = 0; i < nv; i += every) {
        diff(i, /**/ f);
        printf("%g %g\n", Fx[i], f[X]);
    }

    bending_fin(bending);
}

int main(int __UNUSED argc, const char *v[]) {
    int *tri;
    const char path[] = "/dev/stdin";
    static HeOff *off;
    argv = v; argv++;
    arg();

    he_off_ini(path, &off);

    nv = he_off_nv(off);
    nt = he_off_nt(off);
    he_off_tri(off, &tri);
    he_tri_ini(nv, nt, tri, &he);

    MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
    MALLOC(nv, &rxy); MALLOC(nv, &Fm);
    CALLOC(nv, &Fx);  CALLOC(nv, &Fy);  CALLOC(nv, &Fz);

    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(rxy); FREE(Fm);
    FREE(Fx); FREE(Fy); FREE(Fz);

    he_off_fin(off);
    he_fin(he);
}
