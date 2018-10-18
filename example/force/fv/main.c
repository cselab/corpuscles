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
#include <he/vtk.h>
#include <he/area.h>
#define FMT_IN   XE_REAL_IN

static const char **argv;
static char name[4048];


static real Kb, C0, Kad, DA0D;
static real h;
static int  every;
static real *xx, *yy, *zz;
static real *eng, *area;
static real *Fx, *Fy, *Fz, *Fm;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "fv";

static void usg() {
    fprintf(stderr, "%s kantor/gompper/gompper_kroll/juelicher/meyer Kb C0 Kad DA0D step  every < OFF > forces\n", me);
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
    scl(&param.Kb); scl(&param.C0); scl(&param.Kad); scl(&param.DA0D);
    scl(&h);
    num(&every);
    if (every < 1)
      ER("every < 1");
    return HE_OK;
}

static int diff(int i, /**/ real *e0, real *e1) {
    real tmp;
    *e0 = bending_energy(bending, he, xx, yy, zz);
    tmp = xx[i];
    xx[i] += h;
    *e1 = bending_energy(bending, he, xx, yy, zz);
    xx[i] = tmp;
    return HE_OK;
}

static void main0() {
    enum {X, Y, Z};
    int i;
    real f, fd, area0, e0, e1, de;
    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ Fx, Fy, Fz);
    bending_energy_ver(bending, /**/ &eng);
    he_area_ver(he, xx, yy, zz, /**/ area);
    
    for (i = 0; i < nv; i += every) {
        diff(i, /**/ &e0, &e1);
        area0 = area[i];
        fd = Fx[i]/area0;
        de = fd*h*area0;
        printf("%g %g\n", de/h, (e1 - e0)/h);
        Fm[i] = Fx[i] - f;
    }

    if (every == 1) {
        const real *scalars[] = {Fx, Fy, Fz, Fm, NULL};
        const char *names[]   = {"fx", "fy", "fz", "fm", NULL};
        he_vtk_write(he, xx, yy, zz, scalars, names, "q.vtk");
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
    CALLOC(nv, &Fm); CALLOC(nv, &Fx);  CALLOC(nv, &Fy); CALLOC(nv, &Fz);
    MALLOC(nv, &area);

    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(Fm); FREE(Fx); FREE(Fy); FREE(Fz);
    FREE(area);
    

    he_off_fin(off);
    he_fin(he);
}
