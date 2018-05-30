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
    real r0, ang, eng, a[3], b[3], c[3], d[3], r[3];
    real A, *area;
    MALLOC(NV, &area);
    for (v = 0; v < NV; v++) area[v] = 0;
    
    for (v = 0; v < NV; v++) {
        A = 0;
        h0 = h = ver(v);
        do {
            d = hdg(h);
	    i = D0[d]; j = D1[d]; k = D2[d]; l = D3[d];
	    //ai = angle(i, j, k);
	    //al = angle(l, k, j);
	    //e2 = edg(j, k);
	    //A += (ctan(ai) + ctan(al)) * e2;
            //if (h in HDG_BND) break;
	    //f = FLIP_HDG[h]; h = NEXT_HDG[f];
        } while (h != h0);
    }
    
    FREE(area);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
