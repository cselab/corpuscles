#include <stdio.h>
#include <he/err.h>
#include <he/he.h>

static He *he;
static void ini()      { he_file_ini("/dev/stdin", &he); }
static void fin()      { he_fin(he); }

static int  nxt(int h) { return he_nxt(he, h); }
static int  flp(int h) { return he_flp(he, h); }

static int  ver(int h) { return he_ver(he, h); }
static int  edg(int h) { return he_edg(he, h); }
static int  tri(int h) { return he_tri(he, h); }

static int  hdg_ver(int v) { return he_hdg_ver(he, v); }
static int  hdg_edg(int e) { return he_hdg_edg(he, e); }
static int  hdg_tri(int t) { return he_hdg_tri(he, t); }
static int bnd(int h) { return he_bnd(he, h); }

int main() {
    int nv, ne, nt, nh;

    int v, v0, e, e0, t, t0;
    int h, h0, f, cnt;

    ini();
    MSG("%d %d %d %d", nv = he_nv(he), nt = he_nt(he), ne = he_ne(he), nh = he_nh(he));

    for (v = 0; v < nv; v++) { /* from vert and back */
        v0 = ver(hdg_ver(v));
        if (v != v0) ER("v=%d != v0=%d", v, v0);
    }

    for (e = 0; e < ne; e++) { /* from edg and back */
        h = hdg_edg(e);
        e0 = edg(h);
        if (e != e0) ER("%d != %d", e, e0);
    }

    for (t = 0; t < nt; t++) { /* from tri and back */
        h = hdg_tri(t);
        t0 = tri(h);
        if (t != t0) ER("%d != %d", t, t0);
    }

    for (v = 0; v < nv; v++) { /* walk over tri */
        cnt = 0;
        h0 = h = hdg_ver(v);
        do {
            cnt++;
            h = nxt(h);
        } while (h != h0);
        if (cnt != 3) ER("%d != 3 for h=%d", cnt, h0);
    }

    for (h = 0; h < nh; h++) { /* flip-flip */
        if (bnd(h)) continue;
        f = flp(h);
        h0 = flp(f);
        if (h != h0) ER("%d != %d", h, h0);
    }
    fin();

    flp(111111);
}
