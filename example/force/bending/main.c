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

static real *fx, *fy, *fz, *fm, *xx, *yy, *zz, *rr, *eng;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "bending";

static void usg() {
    fprintf(stderr, "%s kantor/gompper/juelicher/meyer/cahnman Kb C0 Kad DA0D < OFF > PUNTO\n", me);
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
static int str(/**/ char *p) {
    if (*argv == NULL) {
        usg();
        ER("not enough args");
    }
    strncpy(p, *argv, 4048);
    argv++;
    return HE_OK;
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

    MSG("energy: %g", e);
    MSG("f0: %g %g %g", fx[0], fy[0], fz[0]);

    for (i = 0; i < nv; i++) {
        vec_get(i, xx, yy, zz, /**/ r);
        vec_get(i, fx, fy, fz, /**/ f);
        rr[i] = vec_cylindrical_r(r);
        fm[i] = vec_abs(f);
    }

    char *key = "r x y z fm fx fy fz eng";
    real *queue[] = {rr, xx, yy, zz, fm, fx, fy, fz, eng, NULL};
    puts(key);
    punto_fwrite(nv, queue, stdout);
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
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(rr); FREE(fm);
    FREE(fx); FREE(fy); FREE(fz);

    he_off_fin(off);
    he_fin(he);
}
