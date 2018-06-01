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
        H[h] = tri_cot(b, c, a);
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
