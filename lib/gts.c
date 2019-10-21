#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "co/err.h"
#include "co/util.h"
#include "co/he.h"
#include "inc/def.h"

#include "co/gts.h"

#define OUT CO_REAL_OUT

int
gts_fwrite(He * he, const real * x, const real * y, const real * z,
           /**/ FILE * f)
{
    int nv, nt, ne, v, e, t, i, j, k, h, n, m;

    nv = he_nv(he);
    ne = he_ne(he);
    nt = he_nt(he);

    fprintf(f, "%d %d %d %s\n", nv, ne, nt,
            "GtsSurface GtsFace GtsEdge GtsVertex");

    for (v = 0; v < nv; v++)
        fprintf(f, OUT " " OUT " " OUT "\n", x[v], y[v], z[v]);
    for (e = 0; e < ne; e++) {
        he_edg_ij(he, e, &i, &j);
        fprintf(f, "%d %d\n", i + 1, j + 1);
    }

    for (t = 0; t < nt; t++) {
        h = he_hdg_tri(he, t);
        n = he_nxt(he, h);
        m = he_nxt(he, n);

        i = he_edg(he, h);
        j = he_edg(he, n);
        k = he_edg(he, m);
        fprintf(f, "%d %d %d\n", k + 1, j + 1, i + 1);
    }
    return CO_OK;
}
