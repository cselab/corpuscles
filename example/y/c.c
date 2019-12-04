#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/multi.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

const char *me = "y/c";
static void
usg()
{
    fprintf(stderr, "%s OFF > struct.c\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
    real *x, *y, *z;
    int *t0, *t1, *t2;
    int nv, nt;

    USED(argc);
    USED(argv);
    y_inif(stdin, &he, &x, &y, &z);

    he_T(he, &t0, &t1, &t2);
    
    y_fin(he, x, y, z);
}
