#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/bending.h>
#include <he/punto.h>
#include <he/he.h>
#include <he/off.h>
#include <he/memory.h>

static real *fx, *fy, *fz, *xx, *yy, *zz;
static int nv, nt;
static He *he;
static Bending *bending;

static void main0() {
    BendingParam param;
    real *queue[] = {xx, yy, zz, fx, fy, fz, NULL};
    param.Kb = 1;
    bending_ini("gompper", param, he,  &bending);
    bending_force(bending, he, xx, yy, zz, /**/ fx, fy, fz);
    punto_fwrite(nv, queue, stdout);
    bending_fin(bending);
}

int main() {
    int *tri;
    const char path[] = "/dev/stdin";
    static HeOff *off;
    he_off_ini(path, &off);

    nv = he_off_nv(off);
    nt = he_off_nt(off);
    he_off_tri(off, &tri);
    he_tri_ini(nv, nt, tri, &he);

    MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);

    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(fx); FREE(fy); FREE(fz);

    he_off_fin(off);
    he_fin(he);
}
