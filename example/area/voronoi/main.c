#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
#include <co/x.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/sum.h>

static void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
    vec_get(l, XX, YY, ZZ, d);
}

static void main1() {
  /*This applies halfedge to traverse*/
    enum {X, Y, Z};
    int v;
    int i, j, k, l;
    int h0, h, n, nn, fnn;
    real a[3], b[3], c[3], d[3], u[3], u2, ci, cl;
    real A, A0;
    Sum *sum;

    sum_ini(&sum);
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
            sum_add(sum, A0);
            h = nxt(flp(h));
        } while (h != h0);
    }

    A = sum_get(sum)/8;

    printf("NT=%i, traverse halfedge A = %.10g\n", NT, A);
    sum_fin(sum);
}

static void main2() {
    /*This utilize edge/diheral to traverse*/
    int e;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3], u2, ci, cl;
    real A, A0;
    Sum *sum;    

    sum_ini(&sum);    
    for (e = 0; e < NE; e++) {
      if (bnd(e)) continue;

      i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
      
      get4(i, j, k, l, /**/ a, b, c, d);
      ci = tri_cot(c, a, b);
      cl = tri_cot(b, d, c);
      vec_minus(b, c,  u);
      u2 = vec_dot(u, u);
      A0 = (ci + cl) * u2;
      sum_add(sum, 2*A0);
    }
    
    A = sum_get(sum)/8;
    printf("NT=%i, traverse edge     A = %.10g\n", NT, A);

    sum_fin(sum);
}


int main() {
    ini("/dev/stdin");
    main1();
    main2();
    fin();
    return CO_OK;
}
