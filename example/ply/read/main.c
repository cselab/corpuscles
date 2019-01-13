#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/macro.h>
#include <he/err.h>
#include <he/he.h>
#include <he/ply.h>
#include <he/off.h>
#include <he/util.h>

static const char **argv;
static Ply *read;
static int nv, nt, nm, m;
const char *me = "ply/read";
real *x, *y, *z;
He *he;

static void usg() {
    fprintf(stderr, "%s [CELL INDEX] < OFF\n", me);
    exit(2);
}

static int num(/**/ int *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, "%d", p) != 1)
        ER("not an integer '%s'", *argv);
    argv++;
    return HE_OK;
}

static void ini() {
    ply_fread(stdin, &read);
}
static void fin() { ply_fin(read); }

static int write() {
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
    return HE_OK;
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    argv = v;
    num(&m);
    ini();
    write();
    fin();
}
