#include <stddef.h>
#include <stdio.h>

#include "real.h"

#include "he/argv.h"
#include "he/err.h"
#include "he/he.h"
#include "he/memory.h"
#include "he/util.h"
#include "he/stretch.h"
#include "he/util.h"

#define T Stretch
#define FMT   HE_REAL_OUT

struct T {
    int nv;
    int n;
    int *plus, *minus;
    real f;
};

int stretch_argv(char ***p, He *he, real *x, real *y, real *z, /**/ T **pq) {
    T *q;
    int status, nv;
    char name[1024];
    real frac, f;

    if ((status = argv_str(p, name)) != HE_OK)
        return status;

    if (!util_eq(name, "plain"))
        ERR(HE_IO, "expecting 'plain' got '%s'", name);

    if (argv_real(p, &frac) != HE_OK)
        ERR(HE_IO, "fail to read fraction");

    if (frac > 0.5)
        ERR(HE_IO, "frac=" FMT " < 0.5", frac);

    if (argv_real(p, &f) != HE_OK)
        ERR(HE_IO, "fail to read force");

    nv = he_nv(he);

    MALLOC(1, &q);

    q->nv = nv;
    q->f = f;

    *pq = q;
    return HE_OK;
}

int stretch_fin(T *q) {
    return HE_OK;
}

int stretch_force(T *q, const real *x, const real *y, const real *z, /*io*/ real *fx, real *fy, real *fz) {
    return HE_OK;
}

int stretch_n(T *q) { return q->n; }
real stretch_f(T *q) { return q->f; }
