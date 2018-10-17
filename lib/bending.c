#include <stdio.h>
#include <stddef.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/container.h"
#include "he/util.h"

#include "he/f/kantor.h"
#include "he/f/juelicher.h"
#include "he/f/gompper.h"
#include "he/f/gompper_kroll.h"
#include "he/f/meyer.h"
#include "he/f/canham.h"

#include "he/bending.h"

#define T Bending

struct T {struct Vtable *vtable; };

static const char *Name[] = {"kantor", "gompper", "gompper_kroll", "juelicher", "meyer", "canham"};
typedef int (*TypeIni)(BendingParam, He*, T**);
static const TypeIni Ini[]  = {bending_kantor_ini, bending_gompper_ini, bending_gompper_kroll_ini, bending_juelicher_ini, bending_meyer_ini, bending_canham_ini};

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

typedef struct Vtable Vtable;
struct Vtable {
    int (*fin)(T*);
    int (*force)(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
    real (*energy)(T*, He*, const real *x, const real *y, const real *z);
    int (*energy_ver)(T*, real**);
};

int bending_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
    return q->vtable->force(q, he, x, y, z, fx, fy, fz);
}
real bending_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    return q->vtable->energy(q, he, x, y, z);
}
int bending_energy_ver(T *q, /**/ real **e) { return q->vtable->energy_ver(q, e); }
int bending_fin(T *q) { return q->vtable->fin(q); }


/* begin kantor */
typedef struct Kantor Kantor;
struct Kantor {T bending; HeFKantor *local; };
static int kantor_fin(T *q) {
    int status;
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    status = he_f_kantor_fin(b->local);
    FREE(q);
    return status;
}
static int kantor_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    return he_f_kantor_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real kantor_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    return he_f_kantor_energy(b->local, he, x, y, z);
}
static int kantor_energy_ver(T *q, /**/ real **e) {
    Kantor *b = CONTAINER_OF(q, Kantor, bending);
    return he_f_kantor_energy_ver(b->local, /**/ e);
}
static Vtable kantor_vtable = { kantor_fin, kantor_force, kantor_energy, kantor_energy_ver};
int bending_kantor_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb;
    Kantor *q;
    Kb = param.Kb;
    MALLOC(1, &q);
    q->bending.vtable = &kantor_vtable;
    *pq = &q->bending;
    return he_f_kantor_ini(Kb, he, &q->local);
}
/* end kantor */


/* begin gompper */
typedef struct Gompper Gompper;
struct Gompper {T bending; HeFGompper *local; };
static int gompper_fin(T *q) {
    int status;
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    status = he_f_gompper_fin(b->local);
    FREE(q);
    return status;
}
static int gompper_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real gompper_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_energy(b->local, he, x, y, z);
}
static int gompper_energy_ver(T *q, /**/ real **e) {
    Gompper *b = CONTAINER_OF(q, Gompper, bending);
    return he_f_gompper_energy_ver(b->local, /**/ e);
}
static Vtable gompper_vtable = { gompper_fin, gompper_force, gompper_energy, gompper_energy_ver};
int bending_gompper_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Gompper *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &gompper_vtable;
    *pq = &q->bending;
    return he_f_gompper_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end gompper */

/* begin gompper_kroll */
typedef struct Gompper_Kroll Gompper_Kroll;
struct Gompper_Kroll {T bending; HeFGompper_Kroll *local; };
static int gompper_kroll_fin(T *q) {
    int status;
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    status = he_f_gompper_kroll_fin(b->local);
    FREE(q);
    return status;
}
static int gompper_kroll_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real gompper_kroll_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_energy(b->local, he, x, y, z);
}
static int gompper_kroll_energy_ver(T *q, /**/ real **e) {
    Gompper_Kroll *b = CONTAINER_OF(q, Gompper_Kroll, bending);
    return he_f_gompper_kroll_energy_ver(b->local, /**/ e);
}
static Vtable gompper_kroll_vtable = { gompper_kroll_fin, gompper_kroll_force, gompper_kroll_energy, gompper_kroll_energy_ver};
int bending_gompper_kroll_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Gompper_Kroll *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &gompper_kroll_vtable;
    *pq = &q->bending;
    return he_f_gompper_kroll_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end gompper_kroll */

/* begin juelicher */
typedef struct Juelicher Juelicher;
struct Juelicher {T bending; HeFJuelicher *local; };
static int juelicher_fin(T *q) {
    int status;
    Juelicher *b = CONTAINER_OF(q, Juelicher, bending);
    status = he_f_juelicher_fin(b->local);
    FREE(q);
    return status;
}
static int juelicher_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Juelicher *b = CONTAINER_OF(q, Juelicher, bending);
    return he_f_juelicher_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real juelicher_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Juelicher *b = CONTAINER_OF(q, Juelicher, bending);
    return he_f_juelicher_energy(b->local, he, x, y, z);
}
static int juelicher_energy_ver(T *q, /**/ real **e) {
    Juelicher *b = CONTAINER_OF(q, Juelicher, bending);
    return he_f_juelicher_energy_ver(b->local, /**/ e);
}
static Vtable juelicher_vtable = { juelicher_fin, juelicher_force, juelicher_energy, juelicher_energy_ver};
int bending_juelicher_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Juelicher *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &juelicher_vtable;
    *pq = &q->bending;
    return he_f_juelicher_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end juelicher */


/* begin meyer */
typedef struct Meyer Meyer;
struct Meyer {T bending; HeFMeyer *local; };
static int meyer_fin(T *q) {
    int status;
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    status = he_f_meyer_fin(b->local);
    FREE(q);
    return status;
}
static int meyer_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real meyer_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_energy(b->local, he, x, y, z);
}
static int meyer_energy_ver(T *q, /**/ real **e) {
    Meyer *b = CONTAINER_OF(q, Meyer, bending);
    return he_f_meyer_energy_ver(b->local, /**/ e);
}
static Vtable meyer_vtable = { meyer_fin, meyer_force, meyer_energy, meyer_energy_ver};
int bending_meyer_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Meyer *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &meyer_vtable;
    *pq = &q->bending;
    return he_f_meyer_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end meyer */


/* begin canham */
typedef struct Canham Canham;
struct Canham {T bending; HeFCanham *local; };
static int canham_fin(T *q) {
    int status;
    Canham *b = CONTAINER_OF(q, Canham, bending);
    status = he_f_canham_fin(b->local);
    FREE(q);
    return status;
}
static int canham_force(T *q, He *he, const real *x, const real *y, const real *z,
                               /**/ real *fx, real *fy, real *fz) {
    Canham *b = CONTAINER_OF(q, Canham, bending);
    return he_f_canham_force(b->local, he, x, y, z, /**/ fx, fy, fz);
}
static real canham_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    Canham *b = CONTAINER_OF(q, Canham, bending);
    return he_f_canham_energy(b->local, he, x, y, z);
}
static int canham_energy_ver(T *q, /**/ real **e) {
    Canham *b = CONTAINER_OF(q, Canham, bending);
    return he_f_canham_energy_ver(b->local, /**/ e);
}
static Vtable canham_vtable = { canham_fin, canham_force, canham_energy, canham_energy_ver};
int bending_canham_ini(BendingParam param, He *he, /**/ T **pq) {
    real Kb, C0, Kad, DA0D;
    Canham *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;
    DA0D = param.DA0D;

    MALLOC(1, &q);
    q->bending.vtable = &canham_vtable;
    *pq = &q->bending;
    return he_f_canham_ini(Kb, C0, Kad, DA0D, he, &q->local);
}
/* end canham */
