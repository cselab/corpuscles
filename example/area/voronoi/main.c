#include <stdio.h>
#include <math.h>

#include <real.h>
#include <he/err.h>
#include <he/x.h>
#include <he/vec.h>
#include <he/tri.h>
#include <he/memory.h>

void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
    vec_get(l, XX, YY, ZZ, d);
}

void main0() {
    enum {X, Y, Z};
    int v, e;
    int i, j, k, l;
    int h0, h;
    real a[3], b[3], c[3], d[3], u[3], u2, ci, cl;
    real A;

    A = 0;
    for (v = 0; v < NV; v++) {
        h0 = h = ver(v);
        do {
            if (bnd(h)) break;
            e = edg(h);
            i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
            get4(i, j, k, l, /**/ a, b, c, d);
            ci = tri_cot(a, b, c);
            cl = tri_cot(d, c, b);
            vec_minus(b, c,  u);
            u2 = vec_dot(u, u);
            A += (ci + cl) * u2;
            h = nxt(flp(h));
        } while (h != h0);
    }
    A /= 8;

    printf("%g\n", A);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
