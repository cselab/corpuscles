#include <stdio.h>

#include <real.h>
#include <he/err.h>
#include <he/off.h>
#include <he/read.h>
#include <he/he.h>
#include <he/macro.h>
#include <he/tri.h>

static const real pi = 3.141592653589793;

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
#define  bnd(h)     he_bnd(he, (h))

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

enum {BULK, BND};
static int get_ijkl(int e, /**/ int *pi, int *pj, int *pk, int *pl) {
    int h, n, nn, nnf, i, j, k, l;
    h = hdg_edg(e);
    if (bnd(h)) return BND;
    n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);
    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}

static int get0(int i, /**/ real a[3]) {
    enum {X, Y, Z};
    a[X] = ver[3*i];
    a[Y] = ver[3*i + 1];
    a[Z] = ver[3*i + 2];
}

static int get(int e, /**/ real a[3], real b[3], real c[3], real d[3]) {
    int status, i, j, k, l;
    status = get_ijkl(e, /**/ &i, &j, &k, &l);
    if (status == BND) return BND;
    get0(i, /**/ a);
    get0(j, /**/ b);
    get0(k, /**/ c);
    get0(l, /**/ d);
    return BULK;
}

static int good(int e) {
    real al, be;
    real a[3], b[3], c[3], d[3];
    get(e, /**/ a, b, c, d);
    al = tri_angle(c, a, b);
    be = tri_angle(b, d, c);
    return al + be < pi;
}

static int equiangulate0(int e) { he_edg_rotate(he, e); }

static void equiangulate() {
    int e, ne, cnt;
    ne = he_ne(he);
    cnt = 0;
    for (e = 0; e < ne; e++) {
        if (!good(e)) {
            equiangulate0(e);
            cnt++;
        }
    }
    MSG("cnt: %d", cnt);
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
