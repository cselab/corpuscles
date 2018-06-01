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
  /*This applies halfedge to traverse*/
    enum {X, Y, Z};
    int v;
    int i, j, k, l;
    int h0, h, n, nn, fnn;
    real a[3], b[3], c[3], d[3], u[3], u2, ci, cl;
    real A, A0;

    A = 0;
    for (v = 0; v < NV; v++) {
        h0 = h = hdg_ver(v);
        do {
            if (bnd(h)) break;
            n = nxt(h); nn = nxt(nxt(h)); fnn = nxt(nxt(flp(h)));
            j = ver(h); k = ver(n); i = ver(nn); l = ver(fnn);
            get4(i, j, k, l, /**/ a, b, c, d);
            ci = tri_cot(c, a, b);
            cl = tri_cot(b, d, c);
            vec_minus(b, c,  u);
            u2 = vec_dot(u, u);
            A0 = (ci + cl) * u2;
            A += A0;
            h = nxt(flp(h));
        } while (h != h0);
    }
    A /= 8;

    printf("NT=%i, traverse halfedge A = %g\n", NT, A);
}

void main1() {
    /*This utilize edge/diheral to traverse*/
    int e;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3], u2, ci, cl;
    real A, A0;

    A = 0;
    for (e = 0; e < NE; e++) {
      if (bnd(e)) continue;

      i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
      
      get4(i, j, k, l, /**/ a, b, c, d);
      ci = tri_cot(c, a, b);
      cl = tri_cot(b, d, c);
      vec_minus(b, c,  u);
      u2 = vec_dot(u, u);
      A0 = (ci + cl) * u2;
      A += 2*A0;
    }
    A /= 8;

    printf("NT=%i, traverse edge     A = %g\n", NT, A);
}


int main() {
    ini("/dev/stdin");
    main0();
    main1();
    fin();
    return HE_OK;
}
