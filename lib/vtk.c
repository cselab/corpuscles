#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "inc/def.h"
#include "co/vtk.h"

#define FMT  CO_REAL_OUT

#define SIZE (MAX_STRING_SIZE)

static int count(const char *a[]);
static int tri_fwrite(He *, const real *, const real *, const real *,
                      const int type[], const char *[], const void *[],
                      /**/ FILE * f);

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

    n_sc = count(names);
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
vtk_tri_write(He * he, const real * x, const real * y, const real * z,
               const real * scalars[], const char *names[], /**/ FILE * f)
{
    int i;
    int n;
    int status;
    int *type;
    const void **data;

    n = count(names);
    MALLOC(n, &type);
    MALLOC(n, &data);
    for (i = 0; i < n; i++) {
        type[i] = VTK_REAL;
        data[i] = (void *) scalars[i];
    }
    status = tri_fwrite(he, x, y, z, type, names, data, f);
    FREE(type);
    return status;
}

int
vtk_tri_int_fwrite(He * he, const real * x, const real * y, const real * z,
		   const int * scalars[], const char *names[], /**/ FILE * f)
{
    int i;
    int n;
    int status;
    int *type;
    const void **data;

    n = count(names);
    MALLOC(n, &type);
    MALLOC(n, &data);
    for (i = 0; i < n; i++) {
        type[i] = VTK_INT;
        data[i] = (void *) scalars[i];
    }
    status = tri_fwrite(he, x, y, z, type, names, data, f);
    FREE(type);
    return status;
}

static int
tri_fwrite(He * he, const real * x, const real * y, const real * z,
           const int type[], const char *names[], const void *scalars[],
           /**/ FILE * f)
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

    n_sc = count(names);
    if (n_sc > 0) {
        fprintf(f, "CELL_DATA %d\n", nt);
        for (i_sc = 0; i_sc < n_sc; i_sc++) {
            if (names[i_sc] == NULL)
                ERR(CO_IO, "not enough names: n_sc=%d, i_sc=%d", n_sc,
                    i_sc);
            switch (type[i_sc]) {
            case VTK_INT:
                fprintf(f, "SCALARS %s int 1\n", names[i_sc]);
                fprintf(f, "LOOKUP_TABLE default\n");
                for (i = 0; i < nt; i++)
                    fprintf(f, "%d\n", ((int **) scalars)[i_sc][i]);
                break;
            case VTK_REAL:
                fprintf(f, "SCALARS %s double 1\n", names[i_sc]);
                fprintf(f, "LOOKUP_TABLE default\n");
                for (i = 0; i < nt; i++)
                    fprintf(f, FMT "\n", ((real **) scalars)[i_sc][i]);
                break;
            default:
                ERR(CO_IO, "unknown type: %d\n", type[i_sc]);
            }
        }
    }
    return CO_OK;
}

static int
count(const char *a[])
{
    int i;

    i = 0;
    if (a == NULL)
        return 0;
    while (a[i] != NULL && i < 999)
        i += 1;
    return i;
}
