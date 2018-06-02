static void mesh_t(real *H) {
    int h, n, f, nn;
    int i, j, k;
    real a[3], b[3], c[3];
    for (h = 0; h < NH; h++) {
        n = nxt(h); nn = nxt(n); f = flp(h);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, /**/ a, b, c);
        H[h] += tri_cot(b, c, a);
        H[f] += tri_cot(b, c, a);
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
        V[i] += T[h]*l2[h]/8;
    }
}

static void mesh_laplace(real *V0, real *T, real *A, /**/ real *V1) {
    int h, n, i, j;
    for (h = 0; h < NH; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += T[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < NV; i++) V1[i] /= A[i];
}

static void mesh_force_t(real *T, real *lx, real *ly, real *lz, /**/
                         real *fx, real *fy, real *fz) {
    int h, n;
    int i, j;
    real r[3], ll[3], df[3];
    real T0, l2;
    for (h = 0; h < NH; h++) {
	n = nxt(h); i = ver(h); j = ver(n);
	T0 = T[h];
	get_edg(i, j,   r);
	vec_get(i, lx, ly, lz, ll);
	l2 = vec_dot(ll, ll);
	vec_linear_combination(-T0/2, ll, T0*l2/8, r,  df);
	vec_append(df, i, /**/ fx, fy, fz);
	vec_substr(df, j, /**/ fx, fy, fz);
    }
}

static void mesh_force_dt(real *lx, real *ly, real *lz, /**/
                          real *fx, real *fy, real *fz) {
    int h, n, nn;
    int i, j, k;
    real r[3], a[3], b[3], c[3], da[3], db[3], dc[3];
    real li[3], lk[3];
    real dl, dd, r2, C;
    for (h = 0; h < NH; h++) {
	n = nxt(h); nn = nxt(n);
	i = ver(h); j = ver(n); k = ver(nn);
	get3(i, j, k,   a, b, c);
	dtri_cot(a, b, c, /**/ da, db, dc);
	get_edg(i, k, /**/ r);
	vec_get(k, lx, ly, lz, /**/ lk);
	vec_get(i, lx, ly, lz, /**/ li);
	r2 = vec_dot(r, r);
	dl = vec_dot(lk, lk) + vec_dot(li, li);
	dd = vec_dot(li, r)  - vec_dot(lk, r);
	C = dd/2 - r2*dl/16;
	vec_scalar_append(da,  C,  i, fx, fy, fz);
	vec_scalar_append(db,  C,  j, fx, fy, fz);
	vec_scalar_append(dc,  C,  k, fx, fy, fz);
    }
}
