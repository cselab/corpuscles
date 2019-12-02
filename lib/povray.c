#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"

#include "co/povray.h"

#define FMT CO_REAL_OUT
#define TAB "  "

int
povray_mesh2(He * he, const real * x, const real * y, const real * z,
             FILE * f)
{
    int nv, nt;
    int i;
    int a, b, c;

    nv = he_nv(he);
    nt = he_nt(he);

    if (fprintf(f, "vertex_vectors {\n") < 0)
        ERR(CO_IO, "fail to write");
    fprintf(f, TAB "%d", nv);
    for (i = 0; i < nv; i++) {
        fprintf(f, ",\n");
        fprintf(f, TAB "<" FMT ", " FMT ", " FMT ">", x[i], y[i], z[i]);
    }
    fprintf(f, "\n}\n");

    fprintf(f, "face_indices {\n");
    fprintf(f, TAB "%d", nt);
    for (i = 0; i < nt; i++) {
        fprintf(f, ",\n");
        he_tri_ijk(he, i, &a, &b, &c);
        fprintf(f, TAB "<%d %d %d>", a, b, c);
    }
    fprintf(f, "\n}\n");
    return CO_OK;
}
