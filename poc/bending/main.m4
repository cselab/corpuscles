include(`he.m4')dnl
h_define(`f2type', `dnl
translit(capitalize(translit($1, `_', ` ')),
         ` ', `')')dnl
h_define(`fun',dnl
`typedef struct f2type($1) f2type($1);
struct f2type($1) {T bending; HeF`'f2type($1) *local; };
static int $1_fin(T *q) {
    int status;
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    status = he_f_$1_fin(b->local);
    FREE(q);
    return status;
}
static int $1_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real $1_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_energy(b->local, he, x, y, z);
}
static int $1_energy_ver(T *q, /**/ real **e) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_energy_ver(b->local, /**/ e);
}
static int $1_area_ver(T *q, /**/ real **e) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_area_ver(b->local, /**/ e);
}
static int $1_curva_mean_ver(T *q, /**/ real **e) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_curva_mean_ver(b->local, /**/ e);
}
static Vtable $1_vtable = { $1_fin, $1_force, $1_energy, $1_energy_ver, $1_area_ver, $1_curva_mean_ver};
')dnl
dnl
h_define(`ini',dnl
`int bending_$1_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    f2type($1) *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &$1_vtable;
    *pq = &q->bending;
    return he_f_$1_ini(Kb, C0, Kad, DA0D, he, &q->local);
}')dnl
dnl
fun(gompper)dnl
ini(gompper)dnl

fun(gompper_kroll)dnl
ini(gompper_kroll)dnl

fun(juelicher_xin)dnl
ini(juelicher_xin)dnl

fun(gompper_xin)dnl
ini(gompper_xin)dnl

fun(meyer_xin)dnl
ini(meyer_xin)dnl
