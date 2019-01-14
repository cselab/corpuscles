#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/macro.h>
#include <he/err.h>
#include <he/ply.h>
#include <he/util.h>

static Ply *read;
static const char *me = "ply/read";

static void usg() {
    fprintf(stderr, "%s [CELL INDEX] < PLY > VTK\n", me);
    exit(2);
}

static void ini() {
    ply_fread(stdin, &read);
}
static void fin() { ply_fin(read); }

static int write() {
    ply_vtk_txt(read, stdout, NULL, NULL);
    return HE_OK;
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    ini();
    write();
    fin();
}
