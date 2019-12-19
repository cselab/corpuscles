#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/read.h>
#include <co/he.h>

static Off *off;
static HeRead *read;
static real *ver;
static int  nv, nt, *tri;
static He *he;

static void ini() {
    off_ini("/dev/stdin", &off);
    nv = off_nv(off);
    nt = off_nt(off);
    off_ver(off, &ver);
    off_tri(off, &tri);
    he_read_tri_ini(nv, nt, tri, &read);
    he_ini(read, &he);
}
static void fin() {
    off_fin(off);
    he_read_fin(read);
    he_fin(he);
}

static void main0() {
    off_he_write(off, he, "/dev/stdout");
}

int main() {
    enum {X, Y, Z};
    ini();
    main0();
    fin();
}
