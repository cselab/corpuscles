#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/err.h>
#include <co/ply.h>
#include <co/util.h>

static const char **argv;
static Ply *read;
static const char *me = "ply/id";
static int nv, nt, nm, m;

static void usg(void) {
    fprintf(stderr, "%s < PLY > VTK\n", me);
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
    real *x, *y, *z;
    int nm, i;
    real *color;
    int *ban;
    
    nm = ply_nm(read);
    MALLOC(nm, &color);
    MALLOC(nm, &ban);
    
    for (i = 0; i < nm; i++) {
        color[i] = i;
        ban[i] = (i != m);
    }

    ply_x(read, m, &x);
    ply_y(read, m, &y);
    ply_z(read, m, &z);
    
    ply_vtk_bin(read, stdout, ban, color);
    FREE(color);
    FREE(ban);
    
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
