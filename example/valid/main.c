#include <stdio.h>
#include <real.h>
#include <co/err.h>
#include <co/x.h>

int main() {
    int v, v0, e, e0, t, t0;
    int h, h0, f, cnt;
    int i, j, k, l;
    int i0, j0, i1, j1;
    int n, nn, nnf;

    ini("/dev/stdin");
    for (v = 0; v < NV; v++) { /* from vert and back */
        v0 = ver(hdg_ver(v));
        if (v != v0) ER("v=%d != v0=%d", v, v0);
    }

    for (e = 0; e < NE; e++) { /* from edg and back */
        h = hdg_edg(e);
        e0 = edg(h);
        if (e != e0) ER("e=%d h=%d e0=%d", e, h, e0);
    }

    for (t = 0; t < NT; t++) { /* from tri and back */
        h = hdg_tri(t);
        t0 = tri(h);
        if (t != t0) ER("%d != %d", t, t0);
    }

    for (v = 0; v < NV; v++) { /* walk over tri */
        cnt = 0;
        h0 = h = hdg_ver(v);
        do {
            cnt++;
            h = nxt(h);
        } while (h != h0);
        if (cnt != 3) ER("%d != 3 for h=%d", cnt, h0);
    }

    for (h = 0; h < NH; h++) { /* flip-flip */
        if (bnd(h)) continue;
        f = flp(h);
        h0 = flp(f);
        if (h != h0) ER("%d != %d", h, h0);
    }

    for (h = 0; h < NH; h++) { if (bnd(h)) continue;
        e = edg(h);
        n = nxt(h); nn = nxt(nxt(h)); nnf = nxt(nxt(flp(h)));
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];

        i0 = ver(nn); i1 = ver(nnf);
        if (i != i0 && i != i1)
            ER("%d is not among [%d, %d]", i, i0, i1);
        if (l != i0 && l != i1)
            ER("%d is not among [%d, %d]", l, i0, i1);

        j0 = ver(h);  j1 = ver(n);
        if (j != j0 && j != j1)
            ER("%d is not among [%d, %d]", j, j0, j1);
        if (k != j0 && k != j1)
            ER("%d is not among [%d, %d]", k, j0, j1);
    }

    fin();
    MSG("ok");
    return(CO_OK);
}
