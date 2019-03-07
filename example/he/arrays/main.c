#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/y.h>

int main() {
    real *x, *y, *z;
    He      *he;
    int *e0, *e1;
    int *d0, *d1, *d2, *d3;
    int i, ne;

    y_inif(stdin, &he, &x, &y, &z);

    he_E(he, &e0, &e1);
    he_D(he, &d0, &d1, &d2, &d3);

    ne = he_ne(he);
    for (i = 0; i < ne; i++) {
        printf("%d %d\n", e0[i], e1[i]);
        printf("%d %d %d %d\n", d0[i], d1[i], d2[i], d3[i]);
        if (e0[i] != d1[i])
            ER("e0[i]=%d   !=   d1[i]=%d", e0[i], d1[i]);
        if (e1[i] != d2[i])
            ER("e1[i]=%d   !=   d2[i]=%d", e1[i], d2[i]);
    }
    y_fin(he,x, y, z);

    return CO_OK;
}
