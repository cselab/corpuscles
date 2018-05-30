#include <stdio.h>
#include <he/err.h>
#include <he/he.h>

static He *he;
static int nv, ne, nt;

static void ini()      { he_file_ini("/dev/stdin", &he); }
static void fin()      { he_fin(he); }

static int  nxt(int h) { return he_nxt(he, h); }
//static int  flp(int h) { return he_flp(he, h); }

static int  ver(int h) { return he_ver(he, h); }
static int  edg(int h) { return he_edg(he, h); }
static int  tri(int h) { return he_tri(he, h); }

static int  hdg_ver(int v) { return he_hdg_ver(he, v); }
static int  hdg_edg(int e) { return he_hdg_edg(he, e); }
static int  hdg_tri(int t) { return he_hdg_tri(he, t); }

static int bnd(int h) { return he_bnd(he, h); }

int main() {
    int v, v0, e, e0, t, t0;
    int h, h0;
    ini();
    MSG("%d %d %d %d", nv = he_nv(he), nt = he_nt(he), ne = he_ne(he), he_nh(he));

    for (v = 0; v < nv; v++) {
        v0 = ver(hdg_ver(v));
        if (v != v0) ERR(HE_IO, "v=%d != v0=%d", v, v0);
    }

    for (e = 0; e < ne; e++) {
        h = hdg_edg(e);
        e0 = edg(h);
        if (e != e0) ERR(HE_IO, "%d != %d", e, e0);
    }

    for (t = 0; t < nt; t++) {
        h = hdg_tri(t);
        t0 = tri(h);
        if (t != t0) ERR(HE_IO, "%d != %d", t, t0);
    }

    for (v = 0; v < nv; v++) {
        h0 = h = hdg_ver(v);
        do {
            printf("%d ", ver(h));
            h = nxt(h);
        } while (h != h0);
        puts("");
    }
    
    fin();
}
