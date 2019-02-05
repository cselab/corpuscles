#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/macro.h>
#include <he/memory.h>
#include <he/err.h>
#include <he/ply.h>
#include <he/util.h>

static Ply *read;
static const char *me = "ply/vtk";

static void usg(void) {
    fprintf(stderr, "%s < PLY > VTK\n", me);
    exit(2);
}

static void ini() {
    ply_fread(stdin, &read);
}
static void fin() { ply_fin(read); }

static int write() {
    int nm, i;
    real *color;
    nm = ply_nm(read);
    MALLOC(nm, &color);
    for (i = 0; i < nm; i++)
        color[i] = i;
    ply_vtk_bin(read, stdout, NULL, color);
    FREE(color);
    return HE_OK;
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    ini();
    write();
    fin();
}
