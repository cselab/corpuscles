#include <stdio.h>

#include "real.h"

#include "he/err.h"
#include "he/memory.h"
#include "he/he.h"
#include "he/off.h"
#include "he/y.h"

int y_ini(const char *path, /**/ He **phe, real **px, real **py, real **pz) {
    HeOff *off;
    int status, nv, nt;
    int *tri;
    He *he;
    real *x, *y, *z;

    status = he_off_ini(path, &off);
    if (status != HE_OK)
        ERR(HE_IO, "he_off_ini failed");

    nv = he_off_nv(off);
    nt = he_off_nt(off);

    status = he_off_tri(off, &tri);
    if (status != HE_OK)
        ERR(HE_IO, "he_off_tri failed");
    
    status = he_tri_ini(nv, nt, tri, &he);
    if (status != HE_OK)
        ERR(HE_IO, "he_tri_ini failed");

    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z);
    he_off_xyz(off, x, y, z);
    he_off_fin(off);

    *phe = he;
    *px = x; *py = y; *pz = z;
    
    return HE_OK;
}

int y_fin(He *he, real *x, real *y, real *z) {
    he_fin(he);
    FREE(x); FREE(y); FREE(z);
    return HE_OK;
}
