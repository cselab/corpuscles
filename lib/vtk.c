#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "real.h"
#include "co/endian.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/util.h"
#include "co/vtk.h"
#include "inc/def.h"

#define FMT  CO_REAL_OUT
#define IN   CO_REAL_IN
#define FMT3 FMT " " FMT " " FMT "\n"
enum { SIZE = MAX_STRING_SIZE };

#define FWRITE(ptr, size)					     \
    do								     \
    if (size != (cnt = fwrite(ptr, sizeof((ptr)[0]), size, f)))	     \
	ERR(CO_IO, "fwrite failed: need = %d, got = %d", size, cnt); \
    while (0)

#define FREAD(n, p)						\
    do								\
	if ((int)fread(p, sizeof(*(p)), (n), (file)) != (n))	\
	    ERR(CO_IO, "fread failed, n = %d", n);		\
    while (0)

#define NXT						\
    do {						\
	util_strcpy(prev, line);			\
	if (util_fgets(line, file) == NULL)		\
	    ERR(CO_IO, "fail to read vtk file");	\
    } while(0)						\

#define EAT(s)							\
    do {							\
	NXT;							\
	if (!util_eq(line, (s)))				\
	    ERR(CO_IO, "expect '%s', got '%s'", (s), line);	\
    } while (0)							\

static int count(const char *a[]);
static int tri_write(He *, const real *, const real *, const real *,
                     const int type[], const char *[], const void *[],
                     /**/ FILE * f);

int
vtk_write(He * he, const real * x, const real * y, const real * z,
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
    fprintf(f, "created with cselab/corpuscles\n");
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
    status = tri_write(he, x, y, z, type, names, data, f);
    FREE(type);
    return status;
}

int
vtk_tri_int_write(He * he, const real * x, const real * y, const real * z,
                  const int *scalars[], const char *names[], /**/ FILE * f)
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
    status = tri_write(he, x, y, z, type, names, data, f);
    FREE(type);
    return status;
}

int
vtk_tri_int_read(FILE * file, const char *names[], He ** phe, real ** px,
                 real ** py, real ** pz, int **scalars[])
{
    char line[SIZE];
    char prev[SIZE];
    char name[SIZE];
    const char *fmt;
    int i;
    int j;
    int nt;
    int nt0;
    int nn;
    int nv;
    int *tri;
    int *scalar;
    int t0;
    int t1;
    int t2;
    real *x;
    real *y;
    real *z;
    He *he;

    line[0] = '\0';
    EAT("# vtk DataFile Version 2.0");
    NXT;                        /* comment */
    EAT("ASCII");
    EAT("DATASET POLYDATA");
    NXT;
    fmt = "POINTS %d double";
    if (sscanf(line, fmt, &nv) != 1)
        ERR(CO_IO, "expected '%s', got '%s'", fmt, line);
    MALLOC3(nv, &x, &y, &z);
    fmt = IN " " IN " " IN;
    for (i = 0; i < nv; i++) {
        NXT;
        if (sscanf(line, fmt, &x[i], &y[i], &z[i]) != 3)
            ERR(CO_IO, "expected '%s', got '%s'", fmt, line);
    }
    NXT;
    fmt = "POLYGONS %d %*d";
    if (sscanf(line, fmt, &nt) != 1)
        ERR(CO_IO, "expected '%s', got '%s'", fmt, line);

    MALLOC(3 * nt, &tri);
    fmt = "%*d %d %d %d";
    for (i = j = 0; i < nt; i++) {
        NXT;
        if (sscanf(line, fmt, &t0, &t1, &t2) != 3)
            ERR(CO_IO, "expected '%s', got '%s'", fmt, line);
        tri[j++] = t0;
        tri[j++] = t1;
        tri[j++] = t2;
    }
    if (he_tri_ini(nv, nt, tri, &he) != CO_OK)
        ERR(CO_IO, "he_tri_in failed (nv=%d, nt=%d)", nv, nt);
    NXT;
    fmt = "CELL_DATA %d";
    if (sscanf(line, fmt, &nt0) != 1)
        ERR(CO_IO, "expected '%s', got '%s'", fmt, line);
    if (nt != nt0)
        ERR(CO_IO, "nt=%d != nt0=%d", nt, nt0);

    nn = count(names);
    for (i = 0; i < nn; i++) {
        NXT;
        fmt = "SCALARS %s int %*d";
        if (sscanf(line, fmt, &name) != 1)
            ERR(CO_IO, "expected '%s', got '%s'", fmt, line);
        if (!util_eq(name, names[i]))
            MSG("%s != %s", name, names[i]);
        EAT("LOOKUP_TABLE default");
        MALLOC(nt, &scalar);
        fmt = "%d";
        for (j = 0; j < nt; j++) {
            NXT;
            if (sscanf(line, "%d", &scalar[j]) != 1)
                ERR(CO_IO, "expected '%s', got '%s'", fmt, line);
        }
        *scalars[i] = scalar;
    }
    FREE(tri);
    *phe = he;
    *px = x;
    *py = y;
    *pz = z;
    return CO_OK;
}


static int
tri_write(He * he, const real * x, const real * y, const real * z,
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
    fprintf(f, "created with cselab/corpuscles\n");
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


int
vtk_grid_write(FILE * f, const int size[3], const real origin[3],
               const real spacing[3], const real * data0)
{
    enum { X, Y, Z };
    const char *me = "co/vtk";
    const char *name = "v";
    int status;
    int n;
    int i;
    int cnt;
    double *data;

    n = size[X] * size[Y] * size[Z];
    MALLOC(n, &data);
    for (i = 0; i < n; i++)
        data[i] = data0[i];
    big_endian_dbl(n, data);
    status = fprintf(f, "# vtk DataFile Version 2.0\n"
                     "generated by %s\n"
                     "BINARY\n"
                     "DATASET STRUCTURED_POINTS\n"
                     "DIMENSIONS %d %d %d\n"
                     "ORIGIN " FMT3
                     "SPACING " FMT3,
                     me, size[X], size[Y], size[Z],
                     origin[X], origin[Y], origin[Z],
                     spacing[X], spacing[Y], spacing[Z]);
    if (status < 0)
        ERR(CO_IO, "vtk_grid_read fail to write a file");
    fprintf(f, "POINT_DATA %d\n"
            "SCALARS %s double\n" "LOOKUP_TABLE DEFAULT\n", n, name);
    FWRITE(data, n);
    FREE(data);
    return CO_OK;
}

int
vtk_grid_read(FILE * file, /**/ int size[3], real origin[3],
              real spacing[3], real ** pdata)
{
    enum { X, Y, Z };
    char line[SIZE];
    char prev[SIZE];
    char name[SIZE];
    const char *fmt;
    real *data;
    int n0;
    int n;
    int i;
    double *data0;

    line[0] = '\0';
    EAT("# vtk DataFile Version 2.0");
    NXT;                        /* comment */
    EAT("BINARY");
    EAT("DATASET STRUCTURED_POINTS");
    NXT;
    fmt = "DIMENSIONS %d %d %d";
    if (sscanf(line, fmt, &size[X], &size[Y], &size[Z]) != 3)
        ERR(CO_OK, "expected '%s', got '%s'", fmt, line);
    NXT;
    fmt = "ORIGIN " IN " " IN " " IN;
    if (sscanf(line, fmt, &origin[X], &origin[Y], &origin[Z]) != 3)
        ERR(CO_OK, "expected '%s', got '%s'", fmt, line);
    NXT;
    fmt = "SPACING " IN " " IN " " IN;
    if (sscanf(line, fmt, &spacing[X], &spacing[Y], &spacing[Z]) != 3)
        ERR(CO_OK, "expected '%s', got '%s'", fmt, line);
    NXT;
    fmt = "POINT_DATA %d";
    if (sscanf(line, fmt, &n0) != 1)
        ERR(CO_OK, "expected '%s', got '%s'", fmt, line);
    NXT;
    fmt = "SCALARS %s double";
    if (sscanf(line, fmt, name) != 1)
        ERR(CO_OK, "expected '%s', got '%s'", fmt, line);
    EAT("LOOKUP_TABLE DEFAULT");
    n = size[X] * size[Y] * size[Z];
    if (n != n0)
        ERR(CO_OK, "n=%d != n0=%d", n, n0);
    MALLOC(n, &data0);
    MALLOC(n, &data);
    FREAD(n, data0);
    big_endian_dbl(n, data0);
    for (i = 0; i < n; i++)
        data[i] = data0[i];
    FREE(data0);
    *pdata = data;
    return CO_OK;
}
