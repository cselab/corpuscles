#include <stdio.h>

#include "real.h"

#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/off.h"
#include "co/y.h"

int
y_inif(FILE * f, /**/ He ** phe, real ** px, real ** py, real ** pz)
{
    HeOff *off;
    int nv, nt;
    int *tri;
    He *he;
    real *x, *y, *z;

    if (off_inif(f, &off) != CO_OK)
        ERR(CO_IO, "off_inif failed");
    nv = off_nv(off);
    nt = off_nt(off);
    if (off_tri(off, &tri) != CO_OK)
        ERR(CO_IO, "off_tri failed");
    if (he_tri_ini(nv, nt, tri, &he) != CO_OK)
        ERR(CO_IO, "he_tri_ini failed");
    MALLOC(nv, &x);
    MALLOC(nv, &y);
    MALLOC(nv, &z);
    off_xyz(off, x, y, z);
    off_fin(off);
    *phe = he;
    *px = x;
    *py = y;
    *pz = z;
    return CO_OK;
}

int
y_inif2(FILE * f0, FILE * f1, /**/ He ** phe, real ** px, real ** py,
        real ** pz)
{
    HeOff *off0, *off1;
    int nv0, nt0, nv1, nt1, nv, nt, i, j;
    int *tri0, *tri1, *tri;
    He *he;
    real *x0, *y0, *z0, *x1, *y1, *z1, *x, *y, *z;

    if (off_inif(f0, &off0) != CO_OK)
        ERR(CO_IO, "off_inif failed");
    if (off_inif(f1, &off1) != CO_OK)
        ERR(CO_IO, "off_inif failed");
    nv0 = off_nv(off0);
    nt0 = off_nt(off0);
    nv1 = off_nv(off1);
    nt1 = off_nt(off1);
    if (off_tri(off0, &tri0) != CO_OK)
        ERR(CO_IO, "off_tri failed");
    if (off_tri(off1, &tri1) != CO_OK)
        ERR(CO_IO, "off_tri failed");
    nt = nt0 + nt1;
    nv = nv0 + nv1;
    MALLOC(3 * nt, &tri);
    j = 0;
    for (i = 0; i < 3 * nt0; i++)
        tri[j++] = tri0[i];
    for (i = 0; i < 3 * nt1; i++)
        tri[j++] = tri1[i] + nv0;
    if (he_tri_ini(nv, nt, tri, &he) != CO_OK)
        ERR(CO_IO, "he_tri_ini failed");
    MALLOC3(nv0, &x0, &y0, &z0);
    MALLOC3(nv1, &x1, &y1, &z1);
    MALLOC3(nv, &x, &y, &z);
    off_xyz(off0, x0, y0, z0);
    off_xyz(off1, x1, y1, z1);
    array_copy3(nv0, x0, y0, z0, x, y, z);
    array_copy3(nv1, x1, y1, z1, &x[nv0], &y[nv0], &z[nv0]);
    FREE(tri);
    FREE3(x0, y0, z0);
    FREE3(x1, y1, z1);
    off_fin(off0);
    off_fin(off1);
    *phe = he;
    *px = x;
    *py = y;
    *pz = z;
    return CO_OK;
}

int
y_ini(const char *path, /**/ He ** he, real ** x, real ** y, real ** z)
{
    FILE *f;

    if ((f = fopen(path, "r")) == NULL)
        ERR(CO_IO, "fail to open '%s'", path);
    if (y_inif(f, he, x, y, z) != CO_OK)
        ERR(CO_IO, "off_fini failed for '%s", path);
    if (fclose(f) != 0)
        ERR(CO_IO, "fail to close '%s'", path);
    return CO_OK;
}

int
y_fin(He * he, real * x, real * y, real * z)
{
    he_fin(he);
    FREE(x);
    FREE(y);
    FREE(z);
    return CO_OK;
}
