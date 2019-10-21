#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "real.h"
#include "inc/def.h"
#include "co/err.h"
#include "co/he.h"

#define FMT  CO_REAL_OUT

#define SIZE (MAX_STRING_SIZE)

static int
count(const real * a[])
{
    int i;

    i = 0;
    if (a == NULL)
        return 0;
    while (a[i] != NULL && i < 999)
        i += 1;
    return i;
}

int
vtk_fwrite(He * he, const real * x, const real * y, const real * z,
           const real * scalars[], const char *names[], /**/ FILE * f)
{
    int np, nv, nt, r, i, n_sc, i_sc;
    int a, b, c;

    np = 3;
    nv = he_nv(he);
    nt = he_nt(he);

    r = fprintf(f, "# vtk DataFile Version 2.0\n");
    if (r <= 0)
        ERR(CO_IO, "fail to write");
    fprintf(f, "created with he\n");
    fprintf(f, "ASCII\n");
    fprintf(f, "DATASET POLYDATA\n");
    fprintf(f, "POINTS %d double\n", nv);
    for (i = 0; i < nv; i++)
        fprintf(f, FMT " " FMT " " FMT "\n", x[i], y[i], z[i]);

    fprintf(f, "POLYGONS %d %d\n", nt, (np + 1) * nt);
    for (i = 0; i < nt; i++) {
        he_tri_ijk(he, i, /**/ &a, &b, &c);
        fprintf(f, "%d %d %d %d\n", np, a, b, c);
    }

    n_sc = count(scalars);
    if (n_sc > 0) {
        fprintf(f, "POINT_DATA %d\n", nv);
        for (i_sc = 0; i_sc < n_sc; i_sc++) {
            if (names[i_sc] == NULL)
                ERR(CO_IO, "not enough names: n_sc=%d, i_sc=%d", n_sc,
                    i_sc);
            fprintf(f, "SCALARS %s double 1\n", names[i_sc]);
            fprintf(f, "LOOKUP_TABLE default\n");
            for (i = 0; i < nv; i++)
                fprintf(f, FMT "\n", scalars[i_sc][i]);
        }
    }
    return CO_OK;
}

int
vtk_write(He * he, const real * x, const real * y, const real * z,
          const real * scalars[], const char *names[],
          /**/ const char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (vtk_fwrite(he, x, y, z, scalars, names, f) != CO_OK)
        ERR(CO_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
vtk_tri_fwrite(He * he, const real * x, const real * y, const real * z,
               const real * scalars[], const char *names[], /**/ FILE * f)
{
    int np, nv, nt, r, i, n_sc, i_sc;
    int a, b, c;

    np = 3;
    nv = he_nv(he);
    nt = he_nt(he);

    r = fprintf(f, "# vtk DataFile Version 2.0\n");
    if (r <= 0)
        ERR(CO_IO, "fail to write");
    fprintf(f, "created with he\n");
    fprintf(f, "ASCII\n");
    fprintf(f, "DATASET POLYDATA\n");
    fprintf(f, "POINTS %d double\n", nv);
    for (i = 0; i < nv; i++)
        fprintf(f, FMT " " FMT " " FMT "\n", x[i], y[i], z[i]);

    fprintf(f, "POLYGONS %d %d\n", nt, (np + 1) * nt);
    for (i = 0; i < nt; i++) {
        he_tri_ijk(he, i, /**/ &a, &b, &c);
        fprintf(f, "%d %d %d %d\n", np, a, b, c);
    }

    n_sc = count(scalars);
    if (n_sc > 0) {
        fprintf(f, "CELL_DATA %d\n", nt);
        for (i_sc = 0; i_sc < n_sc; i_sc++) {
            if (names[i_sc] == NULL)
                ERR(CO_IO, "not enough names: n_sc=%d, i_sc=%d", n_sc,
                    i_sc);
            fprintf(f, "SCALARS %s double 1\n", names[i_sc]);
            fprintf(f, "LOOKUP_TABLE default\n");
            for (i = 0; i < nt; i++)
                fprintf(f, FMT "\n", scalars[i_sc][i]);
        }
    }
    return CO_OK;
}

int
vtk_tri_write(He * he, const real * x, const real * y, const real * z,
              const real * scalars[], const char *names[],
              /**/ const char *path)
{
    FILE *f;

    if ((f = fopen(path, "w")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (vtk_tri_fwrite(he, x, y, z, scalars, names, f) != CO_OK)
        ERR(CO_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}
