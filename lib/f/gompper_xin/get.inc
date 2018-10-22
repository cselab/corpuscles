int he_f_gompper_xin_area_ver(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}
int he_f_gompper_xin_lb_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lbx;
    *py = q->lby;
    *pz = q->lbz;
    return HE_OK;
}
int he_f_gompper_xin_norm_ver(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->normx;
    *py = q->normy;
    *pz = q->normz;
    return HE_OK;
}
int he_f_gompper_xin_H_ver(T *q, /**/ real **pa) {
    *pa = q->H;
    return HE_OK;
}
int he_f_gompper_xin_curva_gauss_ver(T *q, /**/ real **pa) {
    *pa = q->curva_gauss;
    return HE_OK;
}
int he_f_gompper_xin_energy_ver(T *q, /**/ real**pa) {
    *pa = q->energy;
    return HE_OK;
}
