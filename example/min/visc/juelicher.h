static real *lentheta, *AREA, *curva_mean;

typedef struct Juelicher Juelicher;
struct Juelicher { real K; };
static Juelicher juelicher;

static void get3(const real *xx, const real *yy, const real *zz,
                 int i, int j, int k,
                 /**/ real a[3], real b[3], real c[3]) {

    vec_get(i, xx, yy, zz, a);
    vec_get(j, xx, yy, zz, b);
    vec_get(k, xx, yy, zz, c);
}

static void get4(const real *xx, const real *yy, const real *zz,
                 int i, int j, int k, int l, /**/
                 real a[3], real b[3], real c[3], real d[3]) {
    vec_get(i, xx, yy, zz, a);
    vec_get(j, xx, yy, zz, b);
    vec_get(k, xx, yy, zz, c);
    vec_get(l, xx, yy, zz, d);
}

static real f_juelicher_energy(const real *xx, const real *yy, const real *zz) {
    enum {X, Y, Z};
    real Kb;
    int v, e, h, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3], u[3];
    real cur, len, area0;
    real theta;
    real en, e0;
    real C0, H0;

    Kb = juelicher.K;
    C0=0;
    H0=C0/2.0;
    zero(NV, curva_mean); zero(NV, AREA);

    for (e = 0; e < NE; e++) {
        h = hdg_edg(e);
        if ( bnd(h) ) continue;
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta = tri_dih(a, b, c, d);
        vec_minus(b, c, u);
        len = vec_abs(u);
        cur = len*theta/4;
        curva_mean[j] += cur;
        curva_mean[k] += cur;
    }

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c);
        AREA[i] += area0/3;
        AREA[j] += area0/3;
        AREA[k] += area0/3;

    }

    en  = 0;
    for (v = 0; v < NV; v++) {
        curva_mean[v] /= AREA[v];
        e0 = 2 * (curva_mean[v]-H0)*(curva_mean[v]-H0)*AREA[v];
        en  += e0;
    }
    return Kb*en;
}

static int f_juelicher_force(const real *xx, const real *yy, const real *zz,
                  /**/ real *fx, real *fy, real *fz) {
    enum {X, Y, Z};

    real Kb;
    int e, t;
    int i, j, k, l;
    real a[3], b[3], c[3], d[3];
    real da[3], db[3], dc[3], dd[3];
    real u[3];

    real lentheta0, area0, theta0, len0;
    real coef, c0, c1;

    Kb = juelicher.K;

    zero(NV, lentheta); zero(NV, AREA);

    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        vec_minus(c, b, u);
        lentheta0    = vec_abs(u) * tri_dih(a, b, c, d);
        lentheta[j] += lentheta0;
        lentheta[k] += lentheta0;
    }

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(xx, yy, zz, i, j, k, a, b, c);
        area0 = tri_area(a, b, c)/3;
        AREA[i] += area0; AREA[j] += area0; AREA[k] += area0;
    }

    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        theta0 = tri_dih(a, b, c, d);
        dedg_abs(c, b, /**/ dc, db);
        coef = -(lentheta[j]/AREA[j] + lentheta[k]/AREA[k])/4.0*theta0*Kb;
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
    }

    for (e = 0; e < NE; e++) {
        i = D0[e]; j = D1[e]; k = D2[e]; l = D3[e];
        get4(xx, yy, zz, i, j, k, l, /**/ a, b, c, d);
        ddih_angle(a, b, c, d, /**/ da, db, dc, dd);
        vec_minus(c, b, u);
        len0 = vec_abs(u);
        coef = -(lentheta[j]/AREA[j] + lentheta[k]/AREA[k])/4.0*len0*Kb;

        vec_scalar_append(da, coef, i, fx, fy, fz);
        vec_scalar_append(db, coef, j, fx, fy, fz);
        vec_scalar_append(dc, coef, k, fx, fy, fz);
        vec_scalar_append(dd, coef, l, fx, fy, fz);
    }

    for (t = 0; t < NT; t++) {
        i = T0[t]; j = T1[t]; k = T2[t];
        get3(xx, yy, zz, i, j, k, a, b, c);
        dtri_area(a, b, c, /**/ da, db, dc);
        c0 = Kb/24;
        c1 = lentheta[i]*lentheta[i]/AREA[i]/AREA[i] * c0;
        vec_scalar_append(da, c1, i, fx, fy, fz);

        c1 = lentheta[j]*lentheta[j]/AREA[j]/AREA[j] * c0;
        vec_scalar_append(db, c1, j, fx, fy, fz);

        c1 = lentheta[k]*lentheta[k]/AREA[k]/AREA[k] * c0;
        vec_scalar_append(dc, c1, k, fx, fy, fz);
    }
    return HE_OK;
}

static int f_juelicher_ini(float K) {
    juelicher.K = K;
    MALLOC(NV, &lentheta);
    MALLOC(NV, &curva_mean);
    MALLOC(NV, &AREA);
    return HE_OK;
}

static int f_juelicher_fin() {
    FREE(lentheta);
    FREE(curva_mean);
    FREE(AREA);
    return HE_OK;
}
