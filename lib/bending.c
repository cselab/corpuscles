#include <stdio.h>

#include "real.h"

#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"

#include "he/bending.h"

#define T Bending

struct T { };

int bending_ini(BendingParam param, He *he, /**/ T **pq)  {

}

int bending_force(T *q, He *he, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz) {

}

real bending_energy(T *q, He *he, const real *x, const real *y, const real *z) {

}

int bending_energy_ver(T*, /**/ real **e) {

}

int bending_fin(T*) {
}
