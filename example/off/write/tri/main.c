#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>

static Off *off;
static real *ver;
static int  nv, nt, *tri;

static void ini() {
    off_inif(stdin, &off);
    nv = off_nv(off);
    nt = off_nt(off);
    off_ver(off, &ver);
    off_tri(off, &tri);
}
static void fin() { off_fin(off); }

static void main0() {
    off_tri_fwrite(off, tri, stdout);
}

int main() {
    ini();
    main0();
    fin();
}
