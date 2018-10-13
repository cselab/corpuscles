#include <stdio.h>
#include <stddef.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/container.h"

#include "he/f/kantor.h"
#include "he/f/juelicher.h"
#include "he/f/gompper.h"
#include "he/f/meyer.h"

#include "he/bending.h"

#define T Bending

struct T {struct Vtable *vtable; };

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
    int status;
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
    int status;
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
    real Kb, C0, Kad;
    int status;
    Juelicher *q;
    Kb  = param.Kb;
    C0 = param.C0;
    Kad = param.Kad;

    MALLOC(1, &q);
    q->bending.vtable = &juelicher_vtable;
    *pq = &q->bending;
    return he_f_juelicher_ini(Kb, C0, Kad, he, &q->local);
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
    int status;
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
