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
#include <co/area.h>
#include <co/normal.h>

#define FMT_IN   CO_REAL_IN

static const char **argv;
static char name[4048];


static real Kb, C0, Kad, DA0D;
static real h;
static int  every;
static real *xx, *yy, *zz;
static real *eng, *area;
static real *Fx, *Fy, *Fz, *Fm, *nx, *ny, *nz;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "fv";

static void usg(void) {
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

static int diff(int i, /**/ real *e0, real *e1) {
    real tx, ty, tz;
    *e0 = bending_energy(bending, he, xx, yy, zz);
    tx = xx[i]; ty = yy[i]; tz = zz[i];
    xx[i] += h*nx[i]; yy[i] += h*ny[i]; zz[i] += h*nz[i];
    *e1 = bending_energy(bending, he, xx, yy, zz);
    xx[i] = tx;  yy[i] = ty; zz[i] = tz;
    return CO_OK;
}

static void main0() {
    enum {X, Y, Z};
    int i;
    real f, fd, area0, e0, e1, de;
    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ Fx, Fy, Fz);
    bending_energy_ver(bending, /**/ &eng);
    he_area_ver(he, xx, yy, zz, /**/ area);
    normal_mwa(he, xx, yy, zz, /**/ nx, ny, nz);
    for (i = 0; i < nv; i += every) {
        diff(i, /**/ &e0, &e1);
        fd = Fx[i]*nx[i] + Fy[i]*ny[i] + Fz[i]*nz[i];
        de = fd*h;
        printf("%g %g\n", de/h, (e1 - e0)/h);
        Fm[i] = Fx[i] - f;
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
    MALLOC(nv, &nx); MALLOC(nv, &ny); MALLOC(nv, &nz);
    CALLOC(nv, &Fm); CALLOC(nv, &Fx);  CALLOC(nv, &Fy); CALLOC(nv, &Fz);
    MALLOC(nv, &area);

    off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(nx); FREE(ny); FREE(nz);
    FREE(Fm); FREE(Fx); FREE(Fy); FREE(Fz);
    FREE(area);
    

    off_fin(off);
    he_fin(he);
}
