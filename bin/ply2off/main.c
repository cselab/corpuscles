#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/macro.h>
#include <co/err.h>
#include <co/ply.h>
#include <co/off.h>
#include <co/util.h>

static const char *me = "ply2off";
static real *x, *y, *z;
static He *he;

static void
usg(void)
{
    fprintf(stderr, "%s < PLY > OFF\n", me);
    exit(2);
}

static void
ini()
{
    ply_he_ini(stdin, &he, &x, &y, &z);
}

static void
fin()
{
    ply_he_fin(he, x, y, z);
}

static int
write()
{
    off_he_xyz_fwrite(he, x, y, z, stdout);
    return CO_OK;
}

int
main(__UNUSED int c, const char **v)
{
    if (*++v != NULL && util_eq(*v, "-h"))
        usg();
    ini();
    write();
    fin();
}
