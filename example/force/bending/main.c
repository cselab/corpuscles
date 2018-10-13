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
static HeOff *off;
static Bending *bending;

static void main0() {
    BendingParam param;
    real *queue[] = {xx, yy, zz, fx, fy, fz, NULL};
    param.K = 1;
    bending_kantor_ini(param, he,  &bending);
    punto_fwrite(nv, queue, stdout);
    bending_fin(bending);
}

int main() {
    const char path[] = "/dev/stdin";
    he_file_ini(path, &he);
    he_off_ini(path, &off);

    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &xx); MALLOC(nv, &yy); MALLOC(nv, &zz);
    MALLOC(nv, &fx); MALLOC(nv, &fy); MALLOC(nv, &fz);

    he_off_xyz(off, xx, yy, zz);
    main0();

    FREE(xx); FREE(yy); FREE(zz);
    FREE(fx); FREE(fy); FREE(fz);

    he_off_fin(off);
    he_fin(he);
}
