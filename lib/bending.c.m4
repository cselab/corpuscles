include(`co.m4')dnl
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "real.h"

#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/container.h"
#include "co/macro.h"
#include "co/util.h"

#include "co/f/kantor.h"
#include "co/f/juelicher_xin.h"
#include "co/f/gompper.h"
#include "co/f/gompper_kroll.h"
#include "co/f/gompper_xin.h"
#include "co/f/meyer_xin.h"
#include "co/f/meyer.h"

#include "co/bending.h"

#define T Bending

struct T {struct Vtable *vtable;};

#define SIZE (4048)
static char List[SIZE];
typedef int (*TypeIni)(BendingParam, He*, T**);
static const char *Name[] = {
    "gompper",
    "gompper_kroll",
    "gompper_xin",
    "juelicher_xin",
    "kantor",
    "meyer",
    "meyer_xin",
};
static const TypeIni Ini[]  = {
    bending_gompper_ini,
    bending_gompper_kroll_ini,
    bending_gompper_xin_ini,
    bending_juelicher_xin_ini,
    bending_kantor_ini,
    bending_meyer_ini,
    bending_meyer_xin_ini,
};

int bending_ini(const char *name, BendingParam param, He *he, T **pq) {
    const int n = sizeof(Name)/sizeof(Name[0]);
    int i;
    for (i = 0; i < n; i++)
        if (util_eq(name, Name[i]))
            return Ini[i](param, he, pq);

    MSG("unknown bending: '%s'", name);
    MSG("possible values:");
    for (i = 0; i < n; i++)
        MSG("%s", Name[i]);
    ERR(HE_INDEX, "");
}

const char *bending_list() {
    const int n = sizeof(Name)/sizeof(Name[0]);
    const char *sep = "/";
    int i;
    List[0] = '\0';
    for (i = 0; i < n; i++) {
        if (i > 0) strncat(List, sep, SIZE - 1);
        strncat(List, Name[i], SIZE - 1);
    }
    return List;
}

typedef struct Vtable Vtable;
struct Vtable {
    int (*fin)(T*);
    int (*force)(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
    real (*energy)(T*, He*, const real *x, const real *y, const real *z);
    int (*energy_ver)(T*, real**);
    int (*area_ver)(T*, real**);
    int (*curva_mean_ver)(T*, real**);
    real (*energy_bend)(T*);
    real (*energy_ad)(T*);
};

int bending_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
    return q->vtable->force(q, he, x, y, z, fx, fy, fz);
}
real bending_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    return q->vtable->energy(q, he, x, y, z);
}
int bending_energy_ver(T *q, /**/ real **e) { return q->vtable->energy_ver(q, e); }
int bending_area_ver(T *q, /**/ real **e) { return q->vtable->area_ver(q, e); }
int bending_curva_mean_ver(T *q, /**/ real **e) { return q->vtable->curva_mean_ver(q, e); }
real bending_energy_bend(T *q) { return q->vtable->energy_bend(q); }
real bending_energy_ad(T *q) { return q->vtable->energy_ad(q); }
int bending_fin(T *q) { return q->vtable->fin(q); }

h_define(`f2type', `h_translit(capitalize(h_translit($1, `_', ` ')), ` ', `')')dnl
h_define(`fun',dnl
`typedef struct f2type($1) f2type($1);
struct f2type($1) {T bending; HeF`'f2type($1) *local;};
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
static real $1_energy_bend(T *q) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_energy_bend(b->local);
}
static real $1_energy_ad(T *q) {
    f2type($1) *b = CONTAINER_OF(q, f2type($1), bending);
    return he_f_$1_energy_ad(b->local);
}
static Vtable $1_vtable = 
{$1_fin, $1_force, $1_energy, $1_energy_ver, $1_area_ver, $1_curva_mean_ver, $1_energy_bend, $1_energy_ad};
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

fun(kantor)dnl
int bending_kantor_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0;
    Kantor *q;
    Kb = param.Kb;
    C0 = param.C0;
    MALLOC(1, &q);
    q->bending.vtable = &kantor_vtable;
    *pq = &q->bending;
    return he_f_kantor_ini(Kb, C0, he, &q->local);
}

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

fun(meyer)dnl
int bending_meyer_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb;
    Meyer *q;
    Kb  = param.Kb;

    MALLOC(1, &q);
    q->bending.vtable = &meyer_vtable;
    *pq = &q->bending;
    return he_f_meyer_ini(Kb, he, &q->local);
}

