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

static real *xx, *yy, *zz, *rxy, *eng, *area, *cm, *cg, *Fx, *Fy, *Fz, *Fm;
static real *nx, *ny, *nz, *lbx, *lby, *lbz;
static int nv, nt;
static He *he;
static Bending *bending;
static BendingParam param;
static const char *me = "bending";

static void usg() {
    fprintf(stderr, "%s kantor/gompper/gompper_kroll/juelicher/juelicher_xin/meyer/meyer_xin Kb C0 Kad DA0D < OFF > PUNTO\n", me);
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
    real r[3], F[3];
    real e;
    
    bending_ini(name, param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ Fx, Fy, Fz);
    e = bending_energy(bending, he, xx, yy, zz);
    bending_energy_ver(bending, /**/ &eng);
    bending_area_ver(bending, /**/ &area);
    bending_curva_mean_ver(bending, /**/ &cm);
    bending_norm_ver(bending, /**/ &nx, &ny, &nz);
    bending_laplace_ver(bending, /**/ &lbx, &lby, &lbz);
    
    MSG("energy: %g", e);
    MSG("f[0]   : %g %g %g", Fx[0], Fy[0], Fz[0]);
    MSG("f[nv-1]: %g %g %g", Fx[nv-1], Fy[nv-1], Fz[nv-1]);

    for (i = 0; i < nv; i++) {
        vec_get(i, xx, yy, zz, /**/ r);
        vec_get(i, Fx, Fy, Fz, /**/ F);
        rxy[i] = vec_cylindrical_r(r);
        Fm[i]  = vec_abs(F);
    }

    char *key = "#1x 2y 3z 4rxy 5eng 6Fm 7Fx 8Fy 9Fz 10area 11cm 12nx 13ny 14nz 15lbx 16 lby 17 lbz";
    real *queue[] = {xx, yy, zz, rxy, eng, Fm, Fx, Fy, Fz, area, cm, nx, ny, nz, lbx, lby, lbz, NULL};
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
    MALLOC(nv, &rxy);
    CALLOC(nv, &Fx); CALLOC(nv, &Fy); CALLOC(nv, &Fz);
    MALLOC(nv, &Fm);
    
    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(rxy);
    FREE(Fx); FREE(Fy); FREE(Fz);
    FREE(Fm);
    he_off_fin(off);
    he_fin(he);
}
