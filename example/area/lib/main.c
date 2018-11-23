#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/he.h>
#include <he/area.h>
#include <he/memory.h>
#include <he/punto.h>
#include <he/y.h>

int main() {
    He *he;
    int n;
    real *x, *y, *z;
    real A;
    real *area;

    y_ini("/dev/stdin", &he, &x, &y, &z);
    n = he_nt(he);
    MALLOC(n, &area);

    A = he_area(he, x, y, z);
    he_area_tri(he, x, y, z, /**/ area);

    MSG("area: %.16g", A);

    real *queue[] = {area, NULL};
    puts("area");
    punto_fwrite(n, queue, stdout);

    FREE(area);
    y_fin(he, x, y, z);
    return HE_OK;
}
