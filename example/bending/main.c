#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <real.h>
#include <co/area.h>
#include <co/bending.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/util.h>
#include <co/vec.h>

#define FMT_IN   CO_REAL_IN

static const char **argv;
static char name[4048];

static real *fx, *fy, *fz, *fm, *xx, *yy, *zz, *rr, *eng, *area;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "bending";

static void usg(void) {
    const char *list;
    list = bending_list();
    fprintf(stderr, "%s %s Kb C0 Kad DA0D < OFF > PUNTO\n", me, list);
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
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4048);
    argv++;
    return CO_OK;
}
static void arg() {
    if (*argv != NULL && eq(*argv, "-h")) {
        usg();
        exit(0);
    }
    str(name);
    scl(&param.Kb); scl(&param.C0); scl(&param.Kad); scl(&param.DA0D);
}

static void main0() {
    int i;
    real e, r[3], f[3];

    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ fx, fy, fz);
    e = bending_energy(bending, he, xx, yy, zz);
    bending_energy_ver(bending, /**/ &eng);
    he_area_ver(he, xx, yy, zz, /**/ area);

    MSG("energy: %g", e);
    MSG("f0: %g %g %g", fx[0], fy[0], fz[0]);
    MSG("energy_bend: %g", bending_energy_bend(bending));
    MSG("energy_ad: %g", bending_energy_ad(bending));

    for (i = 0; i < nv; i++) {
        vec_get(i, xx, yy, zz, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        fm[i] = vec_abs(f);
    }

    char *key = "r x y z fm fx fy fz eng area";
    const real *queue[] = {rr, xx, yy, zz, fm, fx, fy, fz, eng, area, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);
    bending_fin(bending);
}

int main(int __UNUSED argc, const char *v[]) {
    int *tri;
    const char path[] = "/dev/stdin";
    static Off *off;
    argv = v; argv++;
    arg();

    off_ini(path, &off);

    nv = off_nv(off);
    nt = off_nt(off);
    off_tri(off, &tri);
    he_tri_ini(nv, nt, tri, &he);

    MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
    MALLOC(nv, &rr); MALLOC(nv, &fm); MALLOC(nv, &area);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(rr); FREE(fm); FREE(area);
    FREE(fx); FREE(fy); FREE(fz);

    off_fin(off);
    he_fin(he);
}
