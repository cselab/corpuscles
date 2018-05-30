#include <stdio.h>

#include "real.h"
#include "he/read.h"
#include "he/off.h"
#include "he/err.h"
#include "he/he.h"
#include "he/x.h"

static He *he;

extern int nv, ne, nt, nh;

int ini(const char *path) {
    int nv, nt, *tri;
    HeOff *off;
    HeRead *read;
    
    he_off_ini(path, &off);

    nv = he_off_nv(off);
    nt = he_off_nt(off);

    he_off_tri(off, &tri);
    he_read_tri_ini(nv, nt, tri, &read);
    
    he_ini(read, &he);
    nv = he_nv(he); nt = he_nt(he); ne = he_ne(he); nh = he_nh(he);

    he_read_fin(read);
    he_off_fin(off);
}

int  fin()      { he_fin(he); return HE_OK; }
int  nxt(int h) { return he_nxt(he, h); }
int  flp(int h) { return he_flp(he, h); }

int  ver(int h) { return he_ver(he, h); }
int  edg(int h) { return he_edg(he, h); }
int  tri(int h) { return he_tri(he, h); }

int  hdg_ver(int v) { return he_hdg_ver(he, v); }
int  hdg_edg(int e) { return he_hdg_edg(he, e); }
int  hdg_tri(int t) { return he_hdg_tri(he, t); }
int  bnd(int h)     { return he_bnd(he, h); }
