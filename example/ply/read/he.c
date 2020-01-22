#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/macro.h>
#include <co/err.h>
#include <co/he.h>
#include <co/ply.h>
#include <co/off.h>
#include <co/util.h>

static const char **argv;
static Ply *read;
static int nv, nt, nm, m;
static const char *me = "ply/read";
static real *x, *y, *z;
static He *he;

static void
usg(void)
{
    fprintf(stderr, "%s [CELL INDEX] < PLY > OFF\n", me);
    exit(2);
}

static int
num( /**/ int *p)
{
    if (*argv == NULL)
        ER("not enough args");
    if (sscanf(*argv, "%d", p) != 1)
        ER("not an integer '%s'", *argv);
    argv++;
    return CO_OK;
}

static void
ini()
{
    ply_fread(stdin, &read);
}

static void
fin()
{
    ply_fin(read);
}

static int
write()
{
    nv = ply_nv(read);
    nt = ply_nt(read);
    nm = ply_nm(read);
    ply_he(read, &he);
    MSG("n[vtm]: %d %d %d", nv, nt, nm);
    MSG("n[vth]: %d %d %d", he_nv(he), he_nt(he), he_nh(he));

    ply_x(read, m, &x);
    ply_y(read, m, &y);
    ply_z(read, m, &z);

    off_he_xyz_fwrite(he, x, y, z, stdout);
    return CO_OK;
}

int
main(__UNUSED int c, const char **v)
{
    if (*++v != NULL && util_eq(*v, "-h"))
        usg();
    argv = v;
    num(&m);
    ini();
    write();
    fin();
}
