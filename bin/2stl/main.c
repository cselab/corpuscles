#include <stdio.h>
#include <real.h>
#include <co/stl.h>
#include <co/y.h>

int
main(int argc, char **argv)
{
    static real *x, *y, *z;
    static He *he;

    y_inif(stdin, &he, &x, &y, &z);
    stl_write(he, x, y, z, stdout);

    y_fin(he, x, y, z);
    return 0;
}
