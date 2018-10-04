#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/read.h>
#include <he/he.h>

static HeOff *off;
static HeRead *read;
static real *ver;
static int  nv, nt, *tri;
static He *he;

static void ini() {
    he_off_ini("/dev/stdin", &off);
    nv = he_off_nv(off);
    nt = he_off_nt(off);
    he_off_ver(off, &ver);
    he_off_tri(off, &tri);
    he_read_tri_ini(nv, nt, tri, &read);
    he_ini(read, &he);
}
static void fin() {
    he_off_fin(off);
    he_read_fin(read);
    he_fin(he);
}

static void main0() {
    int e;
    e = 0;
    he_edg_rotate(he, e);

    e = 1;
    he_edg_rotate(he, e);

    he_off_he_write(off, he, "/dev/stdout");
}

int main() {
    enum {X, Y, Z};
    ini();
    main0();
    fin();
}
