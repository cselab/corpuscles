#include <stdio.h>
#include <stdarg.h>

#include "real.h"
#include "he/read.h"
#include "he/off.h"
#include "he/vec.h"
#include "he/err.h"
#include "he/he.h"
#include "he/memory.h"
#include "he/x.h"


static He *he;
int NV, NE, NT, NH;
int *T0, *T1, *T2;
int *D0, *D1, *D2, *D3;
real *XX, *YY, *ZZ;
real *RR;

int  nxt(int h) { return he_nxt(he, h); }
int  flp(int h) { return he_flp(he, h); }

int  ver(int h) { return he_ver(he, h); }
int  edg(int h) { return he_edg(he, h); }
int  tri(int h) { return he_tri(he, h); }

int  hdg_ver(int v) { return he_hdg_ver(he, v); }
int  hdg_edg(int e) { return he_hdg_edg(he, e); }
int  hdg_tri(int t) { return he_hdg_tri(he, t); }
int  bnd(int h)     { return he_bnd(he, h); }

static int rzero(int n, real **pq) {
    int i;
    real *q;
    MALLOC(n, &q);
    for (i = 0; i < n; i++) q[i] = 0;
    *pq = q;
    return HE_OK;
}
int RZERO(int n, ...) {
    int r;
    real **pq;
    va_list ap;
    va_start(ap, n);
    pq = va_arg(ap, real**);
    if ((r = rzero(n, pq)) != HE_OK)
        return r;
    va_end(ap);
    return HE_OK;
}

int ini(const char *path) {
    int i, j, k, l, h, e, n, nn, nnf;
    int *tri, *tri0;
    real *xyz, *xyz0;
    real r[3];
    HeOff *off;
    HeRead *read;

    he_off_ini(path, &off);

    NV = he_off_nv(off);
    NT = he_off_nt(off);

    he_off_tri(off, &tri0);
    he_off_ver(off, &xyz0);
    he_read_tri_ini(NV, NT, tri0, &read);

    he_ini(read, &he);
    NV = he_nv(he); NT = he_nt(he);
    NE = he_ne(he); NH = he_nh(he);

    MALLOC(NV, &XX); MALLOC(NV, &YY); MALLOC(NV, &ZZ);
    MALLOC(NV, &RR);
    MALLOC(NT, &T0); MALLOC(NT, &T1); MALLOC(NT, &T2);
    MALLOC(NE, &D0); MALLOC(NE, &D1); MALLOC(NE, &D2); MALLOC(NE, &D3);

    for (xyz = xyz0, i = 0; i < NV; i++) {
        XX[i] = *xyz++; YY[i] = *xyz++; ZZ[i] = *xyz++;
    }
    for (i = 0; i < NV; i++) {
        vec_get(i, XX, YY, ZZ, r);
        RR[i] = vec_cylindrical_r(r);
    }

    for (tri = tri0, i = 0; i < NT; i++) {
        T0[i] = *tri++; T1[i] = *tri++; T2[i] = *tri++;
    }

    for (e = 0; e < NE; e++) { /* i[jk]l */
        h = hdg_edg(e); n = nxt(h); nn = nxt(nxt(h));
        j = ver(h); k = ver(n); i = ver(nn);
        if (!bnd(h)) {
            nnf = nxt(nxt(flp(h)));
            l = ver(nnf);
        } else l = -1;
        D0[e] = i; D1[e] = j; D2[e] = k; D3[e] = l;
    }

    he_read_fin(read);
    he_off_fin(off);

    return HE_OK;
}

int  fin()      {
    FREE(XX); FREE(YY); FREE(ZZ);
    FREE(T0); FREE(T1); FREE(T2);
    FREE(D0); FREE(D1); FREE(D2); FREE(D3);

    he_fin(he);
    return HE_OK;
}
