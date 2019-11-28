#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/y.h>
#include <co/he.h>

int
main()
{
    He *he;
    real *x, *y, *z;
    int *q, *u, *v, *w, ne, i;

    y_inif(stdin, &he, &x, &y, &z);
    ne = he_ne(he);
    he_D(he, &q, &u, &v, &w);
    for (i = 0; i < ne; i++)
        printf("%d %d %d %d\n", q[i], u[i], v[i], w[i]);
    y_fin(he, x, y, z);
    return CO_OK;
}
