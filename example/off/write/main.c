#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>

static HeOff *off;
static real *ver;
static int  nv, nt, *tri;

static void ini() {
    off_ini("/dev/stdin", &off);
    nv = off_nv(off);
    nt = off_nt(off);
    off_ver(off, &ver);
    off_tri(off, &tri);
}
static void fin() { off_fin(off); }

static void main0() {
    off_tri_write(off, tri, "/dev/stdout");
}

int main() {
    enum {X, Y, Z};
    ini();
    main0();
    fin();
}
