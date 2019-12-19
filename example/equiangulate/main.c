#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/read.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/equiangulate.h>

static const real pi = 3.141592653589793;

static Off *off;
static HeRead *read;
static real *ver;
static int  nv, nt, *tri;
static He *he;
static real *XX, *YY, *ZZ;

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  edg(h)     he_edg(he, (h))
#define  tri(h)     he_tri(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(t) he_hdg_tri(he, (t))
#define  bnd(h)     he_bnd(he, (h))

static void ini() {
    int i;
    off_ini("/dev/stdin", &off);
    nv = off_nv(off);
    nt = off_nt(off);
    off_ver(off, &ver);
    off_tri(off, &tri);
    he_read_tri_ini(nv, nt, tri, &read);
    he_ini(read, &he);
    MALLOC(nv, &XX); MALLOC(nv, &YY); MALLOC(nv, &ZZ);
    for (i = 0; i < nv; i++) {
        XX[i] = *ver++; YY[i] = *ver++; ZZ[i] = *ver++;
    }
}

static void equiangulate() {
    int cnt;
    he_equiangulate(he, XX, YY, ZZ, &cnt);
    MSG("cnt = %d", cnt);
}

static void main0() {
    equiangulate(ver, he);
    off_he_write(off, he, "/dev/stdout");
}

static void fin() {
    FREE(XX); FREE(YY); FREE(ZZ);
    off_fin(off);
    he_read_fin(read);
    he_fin(he);
}

int main() {
    int e;
    ini();
    main0();
    fin();
}
