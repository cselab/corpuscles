#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(e) he_hdg_tri(he, (e))
#define  bnd(h)     he_bnd(he, (h))

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}

static real sum(int n, real *a) {
    int i;
    real v;
    HeSum *sum;
    he_sum_ini(&sum);
    v = 0;
    for (i = 0; i < n; i++)
        he_sum_add(sum, a[i]);
    v = he_sum_get(sum);
    he_sum_fin(sum);
    return v;
}

static int axpy(int n, const real a, const real *x, /*io*/ real *y) {
    int i;
    for (i = 0; i < n; i++)
        y[i] += a*x[i];
    return HE_OK;
}

static int scale(int n, real sc, /*io*/ real *a) {
    int i;
    for (i = 0; i < n; i++)
        a[i] *= sc;
    return HE_OK;
}

static void get_edg(int i, int j, const real *x, const real *y, const real *z, /**/ real r[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, r);
}
static int get3(const real *x, const real *y, const real *z,
                int i, int j, int k,  /**/
                real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return HE_OK;
}

static int get2(const real *x, const real *y, const real *z,
                int i, int j, /**/
                real a[3], real b[3]) {
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    return HE_OK;
}

static int normalize(int n, const real *x, const real *y, const real *z, /**/ real *x0, real *y0, real *z0) {
    int i;
    real a[3], a0[3];
    for (i = 0; i < n; i++) {
        vec_get(i, x, y, z, a);
        vec_norm(a, /**/ a0);
        vec_set(a0, i, /**/ x0, y0, z0);
    }
    return HE_OK;
}
