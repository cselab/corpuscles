#include <stdio.h>

#include "he/err.h"
#include "he/memory.h"
#include "he/read.h"

#include "he/he.h"

#define T He

#define MAGIC (43)

struct T {
    int nv, nt, ne, nh;
    int *nxt, *flp;
    int *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;
    int magic;
};

int he_ini(HeRead *r, T **pq) {
    T *q;
    int nv, nt, ne, nh;
    int *nxt, *flp, *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;
    MALLOC(1, &q);

    nv = he_read_nv(r);
    nt = he_read_nt(r);
    ne = he_read_ne(r);
    nh = he_read_nh(r);    

    MALLOC(nh, &q->nxt); MALLOC(nh, &q->flp);
    MALLOC(nh, &q->ver); MALLOC(nh, &q->tri); MALLOC(nh, &q->edg);
    MALLOC(nv, &q->hdg_ver);
    MALLOC(ne, &q->hdg_edg);
    MALLOC(nt, &q->hdg_tri);

    he_read_nxt(r, &nxt);
    he_read_flp(r, &flp);
    he_read_ver(r, &ver);
    he_read_tri(r, &tri);
    he_read_edg(r, &edg);

    he_read_hdg_ver(r, &hdg_ver);
    he_read_hdg_edg(r, &hdg_edg);
    he_read_hdg_tri(r, &hdg_tri);

    MEMCPY(nh, nxt, &q->nxt); MEMCPY(nh, flp, &q->flp);
    MEMCPY(nh, ver, &q->ver); MEMCPY(nh, tri, &q->tri); MEMCPY(nh, edg, &q->edg);
    MEMCPY(nv, hdg_ver, &q->hdg_ver);
    MEMCPY(ne, hdg_edg, &q->hdg_edg);
    MEMCPY(nt, hdg_tri, &q->hdg_tri);

    q->magic = MAGIC;
    
    *pq = q;
    return HE_OK;    
}

int he_fin(T *q) {
    if (q->magic != MAGIC)
        ERR(HE_MEMORY, "wrong fin() call");
    FREE(q->nxt); FREE(q->flp);
    FREE(q->ver); FREE(q->tri); FREE(q->edg);
    FREE(q->hdg_ver);
    FREE(q->hdg_edg);
    FREE(q->hdg_tri);
    FREE(q);
    FREE(q);
    return HE_OK;
}
