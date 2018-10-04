#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>

static HeOff *off;
static real *ver;
static int  nv, nt, *tri;

static void ini() {
    he_off_ini("/dev/stdin", &off);
    nv = he_off_nv(off);
    nt = he_off_nt(off);
    he_off_ver(off, &ver);
    he_off_tri(off, &tri);
}
static void fin() { he_off_fin(off); }

static void main0() {
    he_off_tri_write(off, tri, "/dev/stdout");
}

int main() {
    enum {X, Y, Z};
    ini();
    main0();
    fin();
}
