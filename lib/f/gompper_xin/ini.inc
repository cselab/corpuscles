int he_f_gompper_xin_ini(real Kb, real C0, __UNUSED real Kad, __UNUSED real DA0D, He *he, T **pq) {
    T *q;
    int nv, ne, nt, nh;
    MALLOC(1, &q);

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);
    nh = he_nh(he);

    q->nv = nv;
    q->ne = ne;
    q->nt = nt;

    q->Kb = Kb;
    q->H0 = C0/2;

    MALLOC(nv, &q->f1x); MALLOC(nv, &q->f1y); MALLOC(nv, &q->f1z);
    MALLOC(nv, &q->f2x); MALLOC(nv, &q->f2y); MALLOC(nv, &q->f2z);
    MALLOC(nh, &q->l2); MALLOC(nh, &q->t);

    MALLOC(nv, &q->lbx); MALLOC(nv, &q->lby); MALLOC(nv, &q->lbz);
    MALLOC(nv, &q->lbx0); MALLOC(nv, &q->lby0); MALLOC(nv, &q->lbz0);

    MALLOC(nv, &q->normx); MALLOC(nv, &q->normy); MALLOC(nv, &q->normz);
    MALLOC(nv, &q->H);  MALLOC(nv, &q->curva_gauss);

    MALLOC(nv, &q->energy);
    MALLOC(nv, &q->area);

    *pq = q;
    return HE_OK;
}
int he_f_gompper_xin_fin(T *q) {
    FREE(q->f1x); FREE(q->f1y); FREE(q->f1z);
    FREE(q->f2x); FREE(q->f2y); FREE(q->f2z);
    
    FREE(q->t); FREE(q->l2);
    FREE(q->lbx); FREE(q->lby); FREE(q->lbz);
    FREE(q->lbx0); FREE(q->lby0); FREE(q->lbz0);

    FREE(q->normx);FREE(q->normy);FREE(q->normz);
    FREE(q->H);FREE(q->curva_gauss);
    FREE(q->energy); FREE(q->area);
    FREE(q);
    return HE_OK;
}
