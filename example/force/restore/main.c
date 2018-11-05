#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/restore.h>
#include <he/he.h>
#include <he/y.h>

static int nv;
static real *fx, *fy, *fz;
static real *x, *y, *z;
static He *he;
static Restore *restore;

static void main0() {
    real *queue[] = {z, y, x, NULL};
    fprintf(stderr, "eng: %g\n", restore_volume(he, x, y, z));
    puts("x y z");
    punto_fwrite(nv, queue, stdout);
}

int main() {
    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);

    he_f_dvolume_ini(he, &dvolume);

    CALLOC(nv, &fx); CALLOC(nv, &fy); CALLOC(nv, &fz);
    main0();

    FREE(fx); FREE(fy); FREE(fz);
    he_f_dvolume_fin(dvolume);

    y_fin(he, x, y, z);
    return 0;
}
