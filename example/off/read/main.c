#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <he/macro.h>
#include <he/err.h>
#include <he/off.h>
#include <he/util.h>

static HeOff *read;
static real *ver;
static int  nv, nt, *tri;

const char *me = "off/read";

static void usg(void) {
    fprintf(stderr, "%s < OFF\n", me);
    exit(2);
}

static void ini() {
    off_ini("/dev/stdin", &read);
    nv = off_nv(read);
    nt = off_nt(read);
    off_ver(read, &ver);
    off_tri(read, &tri);
}
static void fin() { off_fin(read); }

static void write() {
    int i;
    real x, y, z, *r;
    for (i = 0, r = ver; i < nv; i++) {
        x = *r++; y = *r++; z = *r++;
        printf("%g %g %g\n", x, y, z);
    }
    MSG("number of vertices: %d",  nv);
    MSG("number of triangles: %d", nt);
}

int main(__UNUSED int c, const char **v) {
    if (*++v != NULL && util_eq(*v, "-h")) usg();
    ini();
    write();
    fin();
}
