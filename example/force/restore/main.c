#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/restore.h>
#include <he/he.h>
#include <he/y.h>

static int nv;
static real *x, *y, *z;
static He *he;
static Restore *restore;

static void main0() {
    real *queue[] = {z, y, x, NULL};
    restore_volume(restore, he, x, y, z);
    puts("x y z");
    punto_fwrite(nv, queue, stdout);    
}

int main() {
    real V;
    y_ini("/dev/stdin", &he, &x, &y, &z);
    nv = he_nv(he);
    V = 10;
    restore_ini(V, he, &restore);
    main0();
    restore_fin(restore);

    y_fin(he, x, y, z);
    return 0;
}
