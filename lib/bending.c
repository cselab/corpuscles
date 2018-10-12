#include <stdio.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"

#include "he/f/kantor.h"
#include "he/f/juelicher.h"
#include "he/f/gompper.h"
#include "he/f/meyer.h"

#include "he/bending.h"

#define T Bending

struct T {struct Bending_vtable *vtable; };

struct Bending_vtable {
    int (*fin)(T*);
    int (*force)(T*, He*,
                 const real *x, const real *y, const real *z, /**/
                 real *fx, real *fy, real *fz);
    real (*energy)(T*, He*, const real *x, const real *y, const real *z);
    int (*energy_ver)(T*, real**);
};

int bending_ini(const char *name, BendingParam param, He *he, /**/ T **pq)  {
    return HE_OK;
}

int bending_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
    return q->vtable->force(q, he, x, y, z, fx, fy, fz);
}

real bending_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    return q->vtable->energy(q, he, x, y, z);
}

int bending_energy_ver(T *q, /**/ real **e) {
    return q->vtable->energy_ver(q, e);
}

int bending_fin(T *q) {
    return q->vtable->fin(q);
}

struct BendingKantor {
    T bending;
    HeFKantor *local;
};
