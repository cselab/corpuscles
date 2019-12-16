#include <stdlib.h>
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/he/multi.h"

static void u_make(int);
static int u_find(int);
static void u_union(int, int);
static int *u_root;

int
he_multi_label(He * he, int *pnl, int *lbl)
{
#define UNION(t, i)				\
  if (!he_bnd(he, i))				\
    u_union(t, he_tri(he, he_flp(he, i)))

    int nt, nl, t, i, j, k;

    nt = he_nt(he);
    MALLOC(nt, &u_root);
    for (t = 0; t < nt; t++) {
        u_make(t);
        lbl[t] = -1;
    }
    for (t = 0; t < nt; t++) {
        i = he_hdg_tri(he, t);
        j = he_nxt(he, i);
        k = he_nxt(he, j);
        UNION(t, i);
        UNION(t, j);
        UNION(t, k);
    }

    nl = 0;
    for (t = 0; t < nt; t++) {
        i = u_find(t);
        if (lbl[i] == -1)
            lbl[i] = nl++;
        lbl[t] = lbl[i];
    }
    FREE(u_root);
    *pnl = nl;
    return CO_OK;
}

static void
u_make(int v)
{
    u_root[v] = v;
}

static int
u_find(int v)
{
    if (v == u_root[v])
        return v;
    return u_root[v] = u_find(u_root[v]);
}

static void
u_union(int a, int b)
{
    a = u_find(a);
    b = u_find(b);
    if (a != b)
        u_root[b] = a;
}
