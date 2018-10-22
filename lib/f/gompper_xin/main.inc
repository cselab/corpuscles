static void compute_l2(He *he, const real *x, const real *y, const real *z, /**/ real *l2) {
    int nh;
    int h, n;
    int i, j;
    real r[3];
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        get_edg(i, j, x, y, z, /**/ r);
        l2[h] = vec_dot(r, r);
    }
}
static void compute_cot(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], cot;
    nh = he_nh(he);
    zero(nh, H);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(x, y, z, i, j, k, /**/ a, b, c);
        cot = tri_cot(b, c, a);
        H[h] += cot;
        if (!bnd(h)) H[flp(h)] += cot;
    }
}
static void compute_area(He *he, const real *l2, const real *t, /**/ real *V) {
    int nv, nh, h, i;
    nv = he_nv(he);
    zero(nv, V);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        i = ver(h);
        V[i] += t[h]*l2[h]/8;
    }
}
static void compute_lb(He *he, const real *V0, const real *t, const real *area, /**/ real *V1) {
    int h, n, nv, nh, i, j;
    nv = he_nv(he);
    zero(nv, V1);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += t[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < nv; i++)
        V1[i] /= area[i];
}
static int compute_norm(He *he,
                        const real *x, const real *y, const real *z, /**/
                        real *normx, real *normy, real *normz) {
    enum {X, Y, Z};
    int t, nt, h, n, nn;
    int i, j, k, nv;
    real a[3], b[3], c[3], u[3], u0[3];
    real theta_a, theta_b, theta_c;

    nt = he_nt(he);
    nv = he_nv(he);

    zero(nv, normx); zero(nv, normy); zero(nv, normz);
    for ( t = 0; t < nt; t++ ) {
        h = hdg_tri(t);
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);        
        get3(x, y, z, i, j, k, a, b, c);
        theta_a = tri_angle(c, a, b);
        theta_b = tri_angle(a, b, c);
        theta_c = tri_angle(b, c, a);
        tri_normal(a, b, c, u);
        vec_scalar_append(u, theta_a, i, normx, normy, normz);
        vec_scalar_append(u, theta_b, j, normx, normy, normz);
        vec_scalar_append(u, theta_c, k, normx, normy, normz);
    }

    for (i = 0; i < nv; i++) {
        vec_get(i, normx, normy, normz, /**/ u);
        vec_norm(u, /**/ u0);
        vec_set(u0, i, /**/ normx, normy, normz);
    }
    return HE_OK;
}
static int compute_H(He *he,
                     real *lbx, real *lby, real *lbz,
                     real *normx, real *normy, real *normz,
                     /**/ real *H) {
    enum {X, Y, Z};
    int i, nv;
    real u[3], v[3];

    nv  = he_nv(he);

    for ( i = 0; i < nv; i++ ) {
        vec_get(i, lbx, lby, lbz, u);
        vec_get(i, normx, normy, normz, v);
        H[i] = vec_dot(u, v)/2;
    }

    return HE_OK;

}
static int compute_energy(int nv, const real *H, const real *area, /**/ real *energy) {
    int i;
    for (i = 0; i < nv; i++)
        energy[i]   = H[i]*H[i]*area[i];
    return HE_OK;
}

real he_f_gompper_xin_energy(T *q, He *he,
                             const real *x, const real *y, const real *z) {
    int nv, nt;
    int i, j, k, l;
    int *T0, *T1, *T2;

    real Kb;
    real *l2, *t;
    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H;
    real *energy, *area;

    T0 = q->T0; T1 = q->T1; T2 = q->T2;
    l2 = q->l2; t = q->t;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    normx = q->normx; normy = q->normy; normz = q->normz;
    H  = q->H;
    area = q->area; energy = q->energy;
    Kb = q->Kb;

    nv = q->nv;
    nt = he_nt(he);

    for (l = 0; l < nt; l++) {
        get_ijk(l, he, /**/ &i, &j, &k);
        T0[l] = i; T1[l] = j; T2[l] = k;
    }

    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

    compute_l2(he, x, y, z, /**/ l2);
    compute_cot(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_lb(he, x, t, area, /**/ lbx);
    compute_lb(he, y, t, area, /**/ lby);
    compute_lb(he, z, t, area, /**/ lbz);
    compute_norm(he, x, y, z, normx, normy, normz);
    compute_H(he, lbx, lby, lbz, normx, normy, normz, /**/ H);
    compute_energy(nv, H, area, /**/ energy);
    scale(nv, 2*Kb, /**/ energy);

    return sum(nv, energy);
}
static void compute_force_t(He *he,
                            const real *x, const real *y, const real *z,
                            const real *t,
                            const real *lbx, const real *lby, const real *lbz,
                            /**/ real *fx, real *fy, real *fz) {
    int nh;
    int h, n;
    int i, j;
    real r[3], ll[3], df[3];
    real t0, l2;

    nh = he_nh(he);

    for (h = 0; h < nh; h++) {
        n = nxt(h); i = ver(h); j = ver(n);
        t0 = t[h];
        get_edg(i, j, x, y, z, /**/  r);
        vec_get(i, lbx, lby, lbz, ll);
        l2 = vec_dot(ll, ll);
        vec_linear_combination(t0/4, ll, -t0*l2/16, r,  df);
        vec_append(df, i, /**/ fx, fy, fz);
        vec_substr(df, j, /**/ fx, fy, fz);
    }

}
static void compute_force_dt(He *he,
                             const real *x, const real *y, const real *z,
                             const real *lbx, const real *lby, const real *lbz,
                             /**/ real *fx, real *fy, real *fz) {
    int nh;
    int h, n, nn;
    int i, j, k;
    real r[3], a[3], b[3], c[3], da[3], db[3], dc[3];
    real li[3], lk[3];
    real dl, dd, r2, C;

    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(x, y, z, i, j, k,  a, b, c);
        dtri_cot(a, b, c, /**/ da, db, dc);
        get_edg(i, k, x, y, z, /**/ r);
        vec_get(k, lbx, lby, lbz, /**/ lk);
        vec_get(i, lbx, lby, lbz, /**/ li);
        r2 = vec_dot(r, r);
        dl = vec_dot(lk, lk) + vec_dot(li, li);
        dd = vec_dot(li, r)  - vec_dot(lk, r);
        C = dd/4 - r2*dl/32;
        vec_scalar_append(da,  C,  i, /**/ fx, fy, fz);
        vec_scalar_append(db,  C,  j, /**/ fx, fy, fz);
        vec_scalar_append(dc,  C,  k, /**/ fx, fy, fz);
    }

}
int he_f_gompper_xin_force(T *q, He *he,
                           const real *x, const real *y, const real *z, /**/
                           real *fx_tot, real *fy_tot, real *fz_tot) {
    int nv, nt;
    int i, j, k, l;
    int *T0, *T1, *T2;

    real Kb;
    real *l2, *t;
    real *lbx, *lby, *lbz;
    real *area;
    real *fx, *fy, *fz;

    Kb = q->Kb;

    T0 = q->T0; T1 = q->T1; T2 = q->T2;
    area = q->area;
    l2 = q->l2; t = q->t;
    lbx = q->lbx; lby = q->lby; lbz = q->lbz;
    fx = q->fx; fy = q->fy; fz = q->fz;
    nv = q->nv;
    nt = he_nt(he);

    zero(nv, fx); zero(nv, fy); zero(nv, fz);
    
    for (l = 0; l < nt; l++) {
        get_ijk(l, he, /**/ &i, &j, &k);
        T0[l] = i; T1[l] = j; T2[l] = k;
    }

    if (he_nv(he) != nv)
        ERR(HE_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

    compute_l2(he, x, y, z, /**/ l2);
    compute_cot(he, x, y, z, /**/ t);
    compute_area(he, l2, t, /**/ area);
    compute_lb(he, x, t, area, /**/ lbx);
    compute_lb(he, y, t, area, /**/ lby);
    compute_lb(he, z, t, area, /**/ lbz);
    compute_force_t(he, x, y, z, t, lbx, lby, lbz,
                    /**/ fx, fy, fz);
    compute_force_dt(he, x, y, z, lbx, lby, lbz,
                     /**/ fx, fy, fz);

    scale(nv, 2*Kb, fx); scale(nv, 2*Kb, fy); scale(nv, 2*Kb, fz);    

    plus(nv, fx, /*io*/ fx_tot);
    plus(nv, fy, /*io*/ fy_tot);
    plus(nv, fz, /*io*/ fz_tot);    

    return HE_OK;
}
