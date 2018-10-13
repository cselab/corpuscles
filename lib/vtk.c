#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "he/err.h"
#include "he/he.h"

#define FMT_IN   XE_REAL_IN
#define FMT_OUT  "%.16g"

#define SIZE (MAX_STRING_SIZE)

int he_vtk_fwrite(He *he, const real *x, const real *y, const real *z,
                  const real *scalars[], const char *names[], /**/ FILE *f) {
    int nv, nt, r, i;
    nv = he_nv(he);
    nt = he_nt(he);

    r = fprintf(f, "# vtk DataFile Version 2.0\n");
    if (r <= 0) ERR(HE_IO, "fail to write");
    fprintf(f, "created with he\n");
    fprintf(f, "ASCII\n");
    fprintf(f, "DATASET POLYDATA\n");
    fprintf(f, "POINTS %d double\n", nv);
    for (i = 0; i < nv; i++)
        fprintf(f, FMT_OUT " " FMT_OUT " " FMT_OUT "\n",
                x[i], y[i], z[i]);
    return HE_OK;
}

int he_vtk_write(He *he, const real *x, const real *y, const real *z,
                 const real *scalars[], const char *names[], /**/ const char *path) {
    FILE *f;
    if ((f = fopen(path, "w")) == NULL)
        ERR(HE_IO, "fail to open '%s'", path);
    if (he_vtk_fwrite(he, x, y, z, scalars, names, f) != HE_OK)
        ERR(HE_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(HE_IO, "fail to close '%s'", path);
    return HE_OK;
}
