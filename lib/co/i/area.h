static int
get3(const real *x, const real *y, const real *z,
     int i, int j, int k,  /**/
     real a[3], real b[3], real c[3]) {
    i_vec_get(i, x, y, z, /**/ a);
    i_vec_get(j, x, y, z, /**/ b);
    i_vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

static void
zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static int
i_area_ver(He *he, const real *x, const real *y, const real *z, /**/ real *area) {
    int nv, nt, t;
    int i, j, k;
    real area0;
    real a[3], b[3], c[3];

    nt = he_nt(he);
    nv = he_nv(he);

    zero(nv, area);
    for (t = 0; t < nt; t++) {
        he_tri_ijk(he, t, &i, &j, &k);
        get3(x, y, z, i, j, k, a, b, c);
        area0 = i_tri_area(a, b, c)/3;
        area[i] += area0;
        area[j] += area0;
        area[k] += area0;
    }
    return CO_OK;
}
