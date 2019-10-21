#include "real.h"
#include "co/he.h"
#include "co/err.h"
#include "co/distmesh.h"

static int
count(He * he, int i)
{
    int t, j, cnt;

    if (he_bnd(he, i))
        return -1;
    i = he_flp(he, i);
    t = he_tri(he, i);
    j = he_hdg_tri(he, t);
    j = he_nxt(he, j);
    j = he_nxt(he, j);

    for (cnt = 0; i != j; cnt++)
        j = he_nxt(he, j);

    return cnt;
}

int
distmesh_t2n(He * he, /**/ int *out)
{
    int nt, t, i, j, k;

    nt = he_nt(he);
    for (t = 0; t < nt; t++) {
        i = he_hdg_tri(he, t);
        j = he_nxt(he, i);
        k = he_nxt(he, j);

        i = count(he, i);
        j = count(he, j);
        k = count(he, k);
        *out++ = k;             /* to much matlab distmesh order */
        *out++ = i;
        *out++ = j;
    }
    return CO_OK;
}

int
distmesh_t2t(He * he, /**/ int *out)
{
    int nt, t, i, j, k;

    nt = he_nt(he);
    for (t = 0; t < nt; t++) {
        i = he_hdg_tri(he, t);
        j = he_nxt(he, i);
        k = he_nxt(he, j);

        i = he_flp(he, i);
        j = he_flp(he, j);
        k = he_flp(he, k);

        i = he_tri(he, i);
        j = he_tri(he, j);
        k = he_tri(he, k);

        *out++ = k;             /* to much matlab distmesh order */
        *out++ = i;
        *out++ = j;
    }
    return CO_OK;
}
