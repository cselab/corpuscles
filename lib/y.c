#include <stdio.h>

#include "real.h"

#include "co/err.h"
#include "co/he.h
#include "co/memory.h"
#include "co/off.h"
#include "co/y.h"

int y_inif(FILE *f, /**/ He **phe, real **px, real **py, real **pz) {
    HeOff *off;
    int nv, nt;
    int *tri;
    He *he;
    real *x, *y, *z;

    if (off_inif(f, &off) != HE_OK)
        ERR(HE_IO, "off_inif failed");

    nv = off_nv(off);
    nt = off_nt(off);

    if (off_tri(off, &tri) != HE_OK)
        ERR(HE_IO, "off_tri failed");
    
    if (he_tri_ini(nv, nt, tri, &he) != HE_OK)
        ERR(HE_IO, "he_tri_ini failed");

    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z);
    off_xyz(off, x, y, z);
    off_fin(off);

    *phe = he;
    *px = x; *py = y; *pz = z;
    
    return HE_OK;
}

int y_ini(const char *path, /**/ He **he, real **x, real **y, real **z) {
    FILE *f;
    if ((f = fopen(path, "r")) == NULL)
        ERR(HE_IO, "fail to open '%s'", path);
    if (y_inif(f, he, x, y, z) != HE_OK)
        ERR(HE_IO, "off_fini failed for '%s", path);
    if (fclose(f) != 0)
        ERR(HE_IO, "fail to close '%s'", path);
    return HE_OK;
}

int y_fin(He *he, real *x, real *y, real *z) {
    he_fin(he);
    FREE(x); FREE(y); FREE(z);
    return HE_OK;
}
