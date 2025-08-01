#include <stdlib.h>
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/he/multi.h"

static int u_find(int*, int);
static void u_union(int*, int, int);

int
he_multi_label(He * he, int *pnl, int *lbl)
{
#define UNION(t, i)				\
  if (!he_bnd(he, i))				\
    u_union(root, t, he_tri(he, he_flp(he, i)))

    int nt, nl, t, i, j, k;
    int *root;

    nt = he_nt(he);
    MALLOC(nt, &root);
    for (t = 0; t < nt; t++) {
        root[t] = t;
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
      i = u_find(root, t);
        if (lbl[i] == -1)
            lbl[i] = nl++;
        lbl[t] = lbl[i];
    }
    FREE(root);
    *pnl = nl;
    return CO_OK;
}


static int
u_find(int *root, int v)
{
    if (v == root[v])
        return v;
    return root[v] = u_find(root, root[v]);
}

static void
u_union(int *root, int a, int b)
{
    a = u_find(root, a);
    b = u_find(root, b);
    if (a != b)
        root[b] = a;
}
