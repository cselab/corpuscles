#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "real.h"
#include "co/err.h"
#include "co/skel.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/tri.h"
#include "co/vec.h"

#include "co/list/c.h"
#include "co/list/edg1.h"

#define T Edg1List
#define FMT CO_REAL_OUT

struct T {
    int n;
    real lo, hi, size;
    Clist *clist;
};

int
edg1list_ini(const real lo, const real hi, real size, T ** pq)
{
    T *q;
    int n;
    real l;
    Clist *clist;

    MALLOC(1, &q);

    l = hi - lo;
    n = l / size;
    if (n * size < l)
        n++;
    clist_gen_n(n, &clist);
    q->size = size;
    q->lo = lo;
    q->hi = hi;
    q->n = n;
    q->clist = clist;
    *pq = q;
    return CO_OK;
}

int
edg1list_fin(T * q)
{
    clist_fin(q->clist);
    FREE(q);
    return CO_OK;
}

static int
map(T * q, real x, int *i)
{
    real lo, size;

    size = q->size;
    lo = q->lo;
    x -= lo;
    *i = x / size;
    return CO_OK;
}

static int
cm(Skel * skel, const real * x, int m, /**/ real * u)
{
    int i, j;

    skel_edg_ij(skel, m, &i, &j);
    *u = (x[i] + x[j]) / 2;
    return CO_OK;
}

int
edg1list_push(T * q, Skel * skel, const real * x)
{
    int i, m;
    int n;
    real u;

    clist_reset(q->clist);
    n = skel_ne(skel);
    for (m = 0; m < n; m++) {
        cm(skel, x, m, &u);
        map(q, u, &i);
        if (clist_push(q->clist, i, m) != CO_OK) {
            MSG("i: %d", i);
            MSG("n: %d", n);
            ERR(CO_INDEX, "fail to push  traingle " FMT, u);
        }
    }
    return CO_OK;
}

int
edg1list_edgs(T * q, real x, int **pedgs)
{
    int i, n, *edgs;

    n = q->n;
    map(q, x, &i);
    if (clist_parts(q->clist, i, &edgs) != CO_OK) {
        MSG("i: %d", i);
        MSG("n: %d", n);
        ERR(CO_INDEX, "clist_parts failed " FMT, x);
    }
    *pedgs = edgs;
    return CO_OK;
}
