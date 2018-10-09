#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/read.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/tri.h>
#include <he/memory.h>
#include <he/equiangulate.h>

static const real pi = 3.141592653589793;

static HeOff *off;
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
    he_off_ini("/dev/stdin", &off);
    nv = he_off_nv(off);
    nt = he_off_nt(off);
    he_off_ver(off, &ver);
    he_off_tri(off, &tri);
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
    he_off_he_write(off, he, "/dev/stdout");
}

static void fin() {
    FREE(XX); FREE(YY); FREE(ZZ);
    he_off_fin(off);
    he_read_fin(read);
    he_fin(he);
}

int main() {
    int e;
    ini();
    main0();
    fin();
}
