#include <stddef.h>
#include <stdio.h>

#include "real.h"

#include "he/err.h"
#include "he/he.h"
#include "he/memory.h"
#include "he/util.h"
#include "he/stretch.h"

#define T Stretch

struct T {
    int nv;
    int n;
    int *plus, *minus;
    real f;
};

int stretch_argv(char ***v, He *he, real *x, real *y, real *z, /**/ T **pq) {
    T *q;

    *pq = q;
}

int stretch_fin(T *q) {
    return HE_OK;
}

int stretch_force(T *q, const real *x, const real *y, const real *z, /*io*/ real *fx, real *fy, real *fz) {
    return HE_OK;
}

int stretch_n(T *q) { return q->n; }
real stretch_f(T *q) { return q->f; }
