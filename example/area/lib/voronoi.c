#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/area.h>
#include <co/i/vec.h>
#include <co/i/tri.h>
#include <co/i/area.h>
#include <co/memory.h>
#include <co/punto.h>
#include <co/y.h>

int main() {
    He *he;
    int n;
    real *x, *y, *z;
    real A;
    real *area;

    y_inif(stdin, &he, &x, &y, &z);

    n = he_nv(he);
    MALLOC(n, &area);

    A = he_area(he, x, y, z);
    area_ver_voronoi(he, x, y, z, /**/ area);

    MSG("area: " CO_REAL_OUT, A);

    const real *queue[] = {area, NULL};
    puts("area");
    punto_fwrite(n, queue, stdout);

    FREE(area);
    y_fin(he, x, y, z);
    return CO_OK;
}
