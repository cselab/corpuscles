#include <stdio.h>

#include <real.h>
#include <co/area.h>
#include <co/err.h>
#include <co/off.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/y.h>

int main() {
    int nt;
    real *x, *y, *z, *a;
    He *he;
    y_inif(stdin, &he, &x, &y, &z);
    nt = he_nt(he);
    CALLOC(nt, &a);
    he_area_tri(he, x, y, z, a);
    boff_tri_fwrite(he, x, y, z, a, stdout);
    y_fin(he, x, y, z);
    FREE(a);
    return 0;
}
