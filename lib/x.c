#include "he/he.h"

static He *he;
extern int nv, ne, nt, nh;

void ini(const char *path) {
    he_file_ini(path, &he);
    nv = he_nv(he); nt = he_nt(he); ne = he_ne(he); nh = he_nh(he);
}

void fin()      { he_fin(he); }
int  nxt(int h) { return he_nxt(he, h); }
int  flp(int h) { return he_flp(he, h); }

int  ver(int h) { return he_ver(he, h); }
int  edg(int h) { return he_edg(he, h); }
int  tri(int h) { return he_tri(he, h); }

int  hdg_ver(int v) { return he_hdg_ver(he, v); }
int  hdg_edg(int e) { return he_hdg_edg(he, e); }
int  hdg_tri(int t) { return he_hdg_tri(he, t); }
int  bnd(int h)     { return he_bnd(he, h); }
