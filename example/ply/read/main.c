#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/macro.h>
#include <he/err.h>
#include <he/ply.h>
#include <he/util.h>

static Ply *read;
static int nv, nt, nm;
const char *me = "ply/read";

static void usg() {
    fprintf(stderr, "%s < OFF\n", me);
    exit(2);
}

static void ini() {
    ply_fread(stdin, &read);
}
static void fin() { ply_fin(read); }

static int write() {
    nv = ply_nv(read);
    nt = ply_nt(read);
    nm = ply_nm(read);
    MSG("n[vtm]: %d %d %d", nv, nt, nm);
    return HE_OK;
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    ini();
    write();
    fin();
}
