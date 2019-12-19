#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/vtk.h>
#include <co/y.h>

int
main(int argc, char **argv)
{
    static real *x, *y, *z;
    static He *he;

    y_inif(stdin, &he, &x, &y, &z);
    vtk_write(he, x, y, z, NULL, NULL, stdout);

    y_fin(he, x, y, z);
    return 0;
}
