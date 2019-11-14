#include <stdio.h>
#include <assert.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/read.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/equiangulate.h>

static HeOff *off;
static HeRead *read;
static real *ver;
static int  nv, nt, *tri;
static He *he;
static const char **argv;

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  edg(h)     he_edg(he, (h))
#define  tri(h)     he_tri(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(t) he_hdg_tri(he, (t))

static int num(/**/ int *p) {
    if (*argv == NULL) ER("not enough args");
    if (sscanf(*argv, "%d", p) != 1)
        ER("not a number '%s'", *argv);
    argv++;
    return CO_OK;
}

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
    return CO_OK;
}

static int check_ver() {
    int nv, v, h;
    nv = he_nv(he);
    for (v = 0; v < nv; v++) {
        h = hdg_ver(v);
        if (ver(h) != v)
            ER("h=%d v=%d ver(h)=%d", h, v, ver(h));
    }
    return CO_OK;
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
    return CO_OK;
}

static int check_hdgA() {
    int nh, h, n, f, nf;
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        f = flp(h);
        nf = flp(nxt(h));
        if (n == f)
            ER("n=%d   ==   f=%d (h = %d)", n, f, h);
        if (nf == h)
            ER("nf=%d   ==   f=%d (h = %d)", nf, f, h);
    }
    return CO_OK;
}

static int check_hdgB() {
    int nh;
    int h0, h1, h2, h3, h4, h8;
    nh = he_nh(he);
    for (h0 = 0; h0 < nh; h0++) {
        h1 = nxt(h0);
        h2 = nxt(h1);

        h3 = flp(h0);
        h4 = nxt(h3);

        h8 = flp(h4);
        assert(h2 != h8);
    }
    return CO_OK;
}

static void main0(int e) {
    if (!he_ear(he, e)) {
        MSG("rotated: %d", e);
        he_edg_rotate(he, e);
    }
    check_hdgA();
    check_hdgB();
    check_tri();
    check_edg();
    check_ver();
    off_he_write(off, he, "/dev/stdout");
}

int main(int __UNUSED argc, const char *v[]) {
    int e;
    argv = v; argv++;
    num(&e);
    ini();
    main0(e);
    fin();
}
