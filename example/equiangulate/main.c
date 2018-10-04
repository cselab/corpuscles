#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/read.h>
#include <he/he.h>
#include <he/macro.h>

static HeOff *off;
static HeRead *read;
static real *ver;
static int  nv, nt, *tri;
static He *he;

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  edg(h)     he_edg(he, (h))
#define  tri(h)     he_tri(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(t) he_hdg_tri(he, (t))

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

static int check_tri() {
    int h, nh, n, nn, nnn;
    int t;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        nn = nxt(n);
        nnn = nxt(nn);
        if (nnn != h)
            ER("h=%d n=%d nn=%d nnn=%d", h, n, nn, nnn);
        t = tri(h);
        if (tri(n) != t)
            ER("h=%d t=%d tri(n)=%d", h, t, tri(n));
        if (tri(nn) != t)
            ER("h=%d t=%d tri(nn)=%d", h, t, tri(nn));
    }
}

static int check_ver() {
    int nv, v, h;
    nv = he_nv(he);
    for (v = 0; v < nv; v++) {
        h = hdg_ver(v);
        if (ver(h) != v)
            ER("h=%d v=%d ver(h)=%d", h, v, ver(h));
    }
}

static int check_edg() {
    int nh, h, f, ff, e, q;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        f = flp(h);
        ff = flp(f);
        if (ff != h)
            ER("h=%d f=%d ff=%d", h, f, ff);
        e = edg(h);
        if (e != edg(f))
            ER("h=%d f=%d edg(f)=%d", h, f, edg(f));

        q = hdg_edg(e);
        if (q != h && q != f)
            ER("h=%d f=%d e=%d q=%d", h, f, e, q);
    }
}

static int good(int e) {
    return 1;
}

static int equiangulate0(int e) {
    return HE_OK;
}

static void equiangulate() {
    int e, ne;
    ne = he_ne(he);
    for (e = 0; e < ne; e++) {
        if (!good(e)) equiangulate0(e);
    }
}

static void main0() {
    equiangulate(ver, he);
    // he_edg_rotate(he, e);
    check_tri();
    check_edg();
    check_ver();
    he_off_he_write(off, he, "/dev/stdout");
}

int main() {
    int e;
    ini();
    main0();
    fin();
}
