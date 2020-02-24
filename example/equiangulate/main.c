#include <stdio.h>
#include <real.h>
#include <co/equiangulate.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/off.h>
#include <co/y.h>

const char *me = "equiangulate";

int
main(int argc, char **argv)
{
    He *he;
    real *x;
    real *y;
    real *z;
    int cnt;

    USED(argc);
    USED(argv);
    y_inif(stdin, &he, &x, &y, &z);
    he_equiangulate(he, x, y, z, &cnt);
    MSG("%d", cnt);
    if (off_he_xyz_fwrite(he, x, y, z, stdout) != CO_OK)
        ER("off_he_xyz_fwrite failed");
    y_fin(he, x, y, z);
}
