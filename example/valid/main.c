#include <stdio.h>
#include <he/err.h>
#include <he/he.h>

static He *he;
static int nv;

static void ini()      { he_file_ini("/dev/stdin", &he); }
static void fin()      { he_fin(he); }
static int  nxt(int h) { return he_nxt(he, h); }
static int  hdg(int v) { return he_hdg_ver(he, v); }
static int  ver(int h) { return he_ver(he, h); }

int main() {
    int v, h, h0;
    ini();
    MSG("%d %d %d %d", nv = he_nv(he), he_nt(he), he_ne(he), he_nh(he));
    for (v = 0; v < nv; v++) {
        h0 = h = hdg(v);
        do {
            printf("%d ", ver(h));
            h = nxt(h);
        } while (h != h0);
        puts("");
    }
    fin();
}
