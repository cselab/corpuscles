#include <stdio.h>
#include <assert.h>

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

int he_file_ini(const char *path, T **pq) {
    HeRead *read;
    if (he_read_ini(path, &read) != HE_OK)
        ERR(HE_IO, "he_read_ini failed");
    he_ini(read, /**/ pq);
    he_read_fin(read);
    return HE_OK;
}

int he_ini(HeRead *r, T **pq) {
    T *q;
    int nv, nt, ne, nh;
    int *nxt, *flp, *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;
    MALLOC(1, &q);

    nv = q->nv = he_read_nv(r);
    nt = q->nt = he_read_nt(r);
    ne = q->ne = he_read_ne(r);
    nh = q->nh = he_read_nh(r);

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

    MEMCPY(nh, nxt, q->nxt); MEMCPY(nh, flp, q->flp);
    MEMCPY(nh, ver, q->ver); MEMCPY(nh, tri, q->tri); MEMCPY(nh, edg, q->edg);
    MEMCPY(nv, hdg_ver, q->hdg_ver);
    MEMCPY(ne, hdg_edg, q->hdg_edg);
    MEMCPY(nt, hdg_tri, q->hdg_tri);

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
    return HE_OK;
}

int he_fin(T*);

int he_nv(T *q) { return q->nv; }
int he_nt(T *q) { return q->nt; }
int he_ne(T *q) { return q->ne; }
int he_nh(T *q) { return q->nh; }

/* validate */
#define V(i, n) if (0 > (i) || (i) >= n)                        \
        ERR(HE_INDEX, "%s=%d is not in [0, %d)", #i, i, n)
int he_nxt(T *q, int h) { V(h,q->nh); return q->nxt[h]; }
int he_flp(T *q, int h) {
    int f;
    V(h, q->nh);
    if ((f = q->flp[h]) == -1)
        ERR(HE_INDEX, "no flip for %d", h);
    return f;
}
int he_ver(T *q, int h) { V(h, q->nh); return q->ver[h]; }
int he_tri(T *q, int h) { V(h, q->nh); return q->tri[h]; }
int he_edg(T *q, int h) { V(h, q->nh); return q->edg[h]; }
int he_hdg_ver(T *q, int v) { V(v, q->nv); return q->hdg_ver[v]; }
int he_hdg_edg(T *q, int e) { V(e, q->ne); return q->hdg_edg[e]; }
int he_hdg_tri(T *q, int t) { V(t, q->nt); return q->hdg_tri[t]; }
int he_bnd(T *q, int h)     { V(h, q->nh); return q->flp[h] == -1; }


static int set_nxt(T *q, int h, int i) { V(h, q->nh); V(i, q->nh); q->nxt[h] = i; return HE_OK;}
static int set_flp(T *q, int h, int i) { V(h, q->nh); V(i, q->nh); q->flp[h] = i; return HE_OK;}
static int set_ver(T *q, int h, int i) { V(h, q->nh); V(i, q->nv); q->ver[h] = i; return HE_OK;}
static int set_tri(T *q, int h, int i) { V(h, q->nh); V(i, q->nt); q->tri[h] = i; return HE_OK;}
static int set_edg(T *q, int h, int i) { V(h, q->nh); V(i, q->ne); q->edg[h] = i; return HE_OK;}
static int set_hdg_ver(T *q, int v, int i) { V(v, q->nv); V(i, q->nh); q->hdg_ver[v] = i; return HE_OK;}
static int set_hdg_edg(T *q, int e, int i) { V(e, q->ne); V(i, q->nh); q->hdg_edg[e] = i; return HE_OK;}
static int set_hdg_tri(T *q, int t, int i) { V(t, q->nt); V(i, q->nh); q->hdg_tri[t] = i; return HE_OK;}

int he_edg_rotate(T *q, int e0) {
#define  nxt(h)     he_nxt(q, (h))
#define  flp(h)     he_flp(q, (h))
#define  ver(h)     he_ver(q, (h))
#define  edg(h)     he_edg(q, (h))
#define  tri(h)     he_tri(q, (h))
#define  hdg_ver(v) he_hdg_ver(q, (v))
#define  hdg_edg(e) he_hdg_edg(q, (e))
#define  hdg_tri(t) he_hdg_tri(q, (t))

#define  s_nxt(h, i)     set_nxt(q, (h), (i))
#define  s_flp(h, i)     set_flp(q, (h), (i)), set_flp(q, (i), (h))
#define  s_ver(h, i)     set_ver(q, (h), (i))
#define  s_edg(h, i)     set_edg(q, (h), (i))
#define  s_tri(h, i)     set_tri(q, (h), (i))
#define  s_hdg_ver(v, i) set_hdg_ver(q, (v), (i))
#define  s_hdg_edg(e, i) set_hdg_edg(q, (e), (i))
#define  s_hdg_tri(t, i) set_hdg_tri(q, (t), (i))

    int h0, h1, h2, h3, h4, h5, h6, h7, h8, h9;
    int v0, v1, v2, v3;
    int e1, e2, e3, e4;
    int t0, t1;

    V(e0, q->ne);

    h0 = hdg_edg(e0);
    h1 = nxt(h0);
    h2 = nxt(h1);

    h3 = flp(h0);
    h4 = nxt(h3);
    h5 = nxt(h4);

    h6 = flp(h1);
    h7 = flp(h2);
    h8 = flp(h4);
    h9 = flp(h5);

    v0 = ver(h0);
    v1 = ver(h1);
    v2 = ver(h2);
    v3 = ver(h5); /* sic */

    e1 = edg(h1);
    e2 = edg(h2);
    e3 = edg(h4); /* sic */
    e4 = edg(h5);

    t0 = tri(h0);
    t1 = tri(h3);

    assert(v0 == ver(h4)); assert(v0 == ver(h7));
    assert(v1 == ver(h3)); assert(v1 == ver(h9));
    assert(v2 == ver(h6));
    assert(v3 == ver(h8));

    assert(e0 == edg(h3));
    assert(e1 == edg(h6));
    assert(e2 == edg(h7));
    assert(e3 == edg(h8));
    assert(e4 == edg(h9));

    assert(t0 == tri(h1)); assert(t0 == tri(h2));
    assert(t1 == tri(h4)); assert(t1 == tri(h5));

    s_nxt(h0, h1);
    s_nxt(h1, h2);
    s_nxt(h2, h0);
    s_nxt(h3, h4);
    s_nxt(h4, h5);
    s_nxt(h5, h3);

    s_flp(h0, h3);
    s_flp(h1, h7);
    s_flp(h2, h8);
    s_flp(h4, h9);
    s_flp(h5, h6);

    s_ver(h0, v3);
    s_ver(h1, v2);
    s_ver(h2, v0);
    s_ver(h3, v2);
    s_ver(h4, v3);
    s_ver(h5, v1);
    s_ver(h6, v2);
    s_ver(h7, v0);
    s_ver(h8, v3);
    s_ver(h9, v1);

    s_edg(h0, e0);
    s_edg(h1, e2);
    s_edg(h2, e3);

    s_edg(h3, e0);
    s_edg(h4, e4);
    s_edg(h5, e1);

    s_edg(h6, e1);
    s_edg(h7, e2);
    s_edg(h8, e3);
    s_edg(h9, e4);

    s_tri(h0, t0);
    s_tri(h1, t0);
    s_tri(h2, t0);

    s_tri(h3, t1);
    s_tri(h4, t1);
    s_tri(h5, t1);

    s_hdg_ver(v0, h2);
    s_hdg_ver(v1, h9);
    s_hdg_ver(v2, h6);
    s_hdg_ver(v3, h4);

    s_hdg_edg(e0, h0);
    s_hdg_edg(e1, h5);
    s_hdg_edg(e2, h1);
    s_hdg_edg(v3, h2);

    s_hdg_tri(t0, h0);
    s_hdg_tri(t1, h3);

    assert(v0 == ver(h2)); assert(v0 == ver(h7));
    assert(v1 == ver(h5)); assert(v1 == ver(h9));
    assert(v2 == ver(h6));
    assert(v3 == ver(h8));

    assert(e0 == edg(h3));
    assert(e1 == edg(h6));
    assert(e2 == edg(h7));
    assert(e3 == edg(h8));
    assert(e4 == edg(h9));

    return HE_OK;
}
