#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/bending.h>
#include <he/punto.h>
#include <he/read.h>
#include <he/he.h>

static real *FX, *FY, *FZ, *XX, *YY, *ZZ;
static int nv, nt;
static He *he;
static Bending *bending;

static void main0() {
    BendingParam param;
    real *queue[] = {XX, YY, ZZ, FX, FY, FZ, NULL};
    param.K = 1;
    bending_kantor_ini(param, he,  &bending);
    punto_fwrite(nv, queue, stdout);
    bending_fin(bending);
}

int main() {
    HeRead *read;
    const char path[] = "/dev/stdin";
    he_read_ini(path, &read);
    he_ini(read, &he);
    nv = he_nv(he);
    nt = he_nt(he);
    main0();
    he_read_fin(read);
    he_fin(he);
}
