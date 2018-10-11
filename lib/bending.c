#include <stdio.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"

#include "he/bending.h"

#define T Bending


struct T {struct Bending_vtable *vtable; };

struct Bending_vtable {
    int (*ini)(BendingParam, He*, /**/  T**);
    int (*fin)(T*);
    int (*force)(T*, He*,
                 const real *x, const real *y, const real *z, /**/
                 real *fx, real *fy, real *fz);
    real (*energy)(T*, He*, const real *x, const real *y, const real *z);
    int (*energy_ver)(T*, real**);
};

int bending_ini(BendingParam param, He *he, /**/ T **pq)  {
    return HE_OK;
}

int bending_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {
    return HE_OK;
}

real bending_energy(T *q, He *he, const real *x, const real *y, const real *z) {
    return HE_OK;
}

int bending_energy_ver(T *q, /**/ real **e) {
    return HE_OK;

}

int bending_fin(T *q) {
    return HE_OK;    
}
