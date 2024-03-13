#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <real.h>
#include <co/err.h>
/* #include <co/x.h> */
#include <co/vec.h>
#include <co/tri.h>
#include <co/memory.h>
#include <co/punto.h>

static void get3(int i, int j, int k, /**/
          real a[3], real b[3], real c[3]) {
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_get(k, XX, YY, ZZ, c);
}

static void get_edg(int i, int j, real r[3]) {
    real a[3], b[3];
    vec_get(i, XX, YY, ZZ, a);
    vec_get(j, XX, YY, ZZ, b);
    vec_minus(a, b, r);
}

static void mesh_T(real *cot, /**/ real *H) {
    int h, f;
    real T;
    for (h = 0; h < NH; h++) {
        T  = cot[h];
        if ((f = flp(h)) != -1)
            T += cot[f];
        H[h] = T;
    }
}

static void mesh_cot(real *H) {
    int h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3];
    for (h = 0; h < NH; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, /**/ a, b, c);
        H[h] = tri_cot(a, b, c);
    }
}

static void mesh_l2(real *H) {
    int h, n;
    int i, j;
    real r[3];
    for (h = 0; h < NH; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        get_edg(i, j, /**/ r);
        H[h] = vec_dot(r, r);
    }
}

static void mesh_voronoi(real *T, real *l2, /**/ real *V) {
    int h, i;
    for (h = 0; h < NH; h++) {
        i = ver(h);
        V[i] += T[h]*l2[h]/4;
    }
}

static void mesh_laplace(real *V0, real *T, real *A, /**/ real *V1) {
    int h, n, i, j;
    for (h = 0; h < NH; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += T[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < NV; i++)
        V1[i] /= A[i];
}

static real sum(int n, real *a) {
    int i;
    real s;
    for (i = 0, s = 0; i < n; i++)
        s += a[i];
    return s;
}

static void main0() {
    real *cot, *l2, *A, *T, *LX, *LY, *LZ;

    RZERO(NH, &cot);
    RZERO(NH, &l2); RZERO(NH, &T);
    RZERO(NV, &A); RZERO(NV, &LX); RZERO(NV, &LY); RZERO(NV, &LZ);

    const real *queue[] = {RR, TH, LX, LY, LZ, NULL};

    mesh_cot(cot);
    mesh_T(cot, /**/ T);
    mesh_l2(l2);
    mesh_voronoi(cot, l2, /**/ A);

    mesh_laplace(XX, T, A, /**/ LX);
    mesh_laplace(YY, T, A, /**/ LY);
    mesh_laplace(ZZ, T, A, /**/ LZ);

    MSG("%g", sum(NV, A));
    punto_write(NV, queue, "/dev/stdout");

    FREE(cot);
    FREE(l2); FREE(T); FREE(A);
    FREE(LX); FREE(LY); FREE(LZ);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return CO_OK;
}
