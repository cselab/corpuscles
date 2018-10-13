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
    real K;
    int status;
    Kantor *q;
    K = param.K;
    MALLOC(1, &q);
    q->bending.vtable = &kantor_vtable;
    *pq = &q->bending;
    return he_f_kantor_ini(K, he, &q->local);
}
/* end kantor */
