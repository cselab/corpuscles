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

static real *fx, *fy, *fz, *fm, *xx, *yy, *zz, *rr, *eng, h;
static int nv, nt, every;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "fd";

static void usg() {
    fprintf(stderr, "%s kantor/gompper/juelicher/meyer/canham step  every < OFF > forces\n", me);
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
    str(name); scl(&h); num(&every);
    if (every < 1)
        ER("ever < 1");
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
    param.Kb = 1;
    param.C0 = 1;
    param.Kad = 2;
    param.DA0D = 0;

    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ fx, fy, fz);

    e = bending_energy(bending, he, xx, yy, zz);
    bending_energy_ver(bending, /**/ &eng);

    MSG("energy: %g", e);
    for (i = 0; i < nv; i += every) {
        diff(i, /**/ f);
        printf("%g %g\n", fx[i], f[X]);
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
    MALLOC(nv, &rr); MALLOC(nv, &fm);
    CALLOC(nv, &fx);  CALLOC(nv, &fy);  CALLOC(nv, &fz);

    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(rr); FREE(fm);
    FREE(fx); FREE(fy); FREE(fz);

    he_off_fin(off);
    he_fin(he);
}
