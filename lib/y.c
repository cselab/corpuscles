#include <stdio.h>

#include "real.h"

#include "he/err.h"
#include "he/memory.h"
#include "he/he.h"
#include "he/off.h"
#include "he/y.h"

int y_ini(const char *path, /**/ He **he, real **x, real **y, real **z) {
    HeOff *off;    
    int nv, nt;
    int *tri;
    
    he_off_ini(path, &off);

    nv = he_off_nv(off);
    nt = he_off_nt(off);
    
    he_off_tri(off, &tri);
    he_tri_ini(nv, nt, tri, &he);

    MALLOC(nv, &x); MALLOC(nv, &y); MALLOC(nv, &z);
    he_off_xyz(off, x, y, z);

    he_off_fin(off);
    return HE_OK;
}

int y_fin(He *he, real *x, real *y, real *z) {
    he_fin(he);
    FREE(x); FREE(y); FREE(z);
    return HE_OK;
}
