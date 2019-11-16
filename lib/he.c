#include <stdio.h>
#include <assert.h>

#include "co/err.h"
#include "co/memory.h"
#include "co/read.h"
#include "inc/def.h"

#include "co/he.h"

#define T He

#define MAGIC (43)

struct T {
    int nv, nt, ne, nh;
    int *nxt, *flp;
    int *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;
    int *E0, *E1;
    int *T0, *T1, *T2;
    int *D0, *D1, *D2, *D3;
    int ring[RANK_MAX + 1];
    int magic;
};

enum { END = -1 };
static int
distinct(const int a[])
{
    int i, j;

    for (i = 0; a[i] != END; i++)
        for (j = i + 1; a[j] != END; j++)
            if (a[i] == a[j])
                return 0;
    return 1;
}

int
he_file_ini(const char *path, T ** pq)
{
    HeRead *read;

    if (he_read_ini(path, &read) != CO_OK)
        ERR(CO_IO, "he_read_ini failed");
    he_ini(read, /**/ pq);
    he_read_fin(read);
    return CO_OK;
}

int
he_ini(HeRead * r, T ** pq)
{
    T *q;
    int nv, nt, ne, nh;
    int *nxt, *flp, *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;

    MALLOC(1, &q);

    nv = q->nv = he_read_nv(r);
    nt = q->nt = he_read_nt(r);
    ne = q->ne = he_read_ne(r);
    nh = q->nh = he_read_nh(r);

    MALLOC(nh, &q->nxt);
    MALLOC(nh, &q->flp);
    MALLOC(nh, &q->ver);
    MALLOC(nh, &q->tri);
    MALLOC(nh, &q->edg);
    MALLOC(nv, &q->hdg_ver);
    MALLOC(ne, &q->hdg_edg);
    MALLOC(nt, &q->hdg_tri);

    MALLOC(ne, &q->E0);
    MALLOC(ne, &q->E1);
    MALLOC(nt, &q->T0);
    MALLOC(nt, &q->T1);
    MALLOC(nt, &q->T2);
    MALLOC(ne, &q->D0);
    MALLOC(ne, &q->D1);
    MALLOC(ne, &q->D2);
    MALLOC(ne, &q->D3);

    he_read_nxt(r, &nxt);
    he_read_flp(r, &flp);
    he_read_ver(r, &ver);
    he_read_tri(r, &tri);
    he_read_edg(r, &edg);

    he_read_hdg_ver(r, &hdg_ver);
    he_read_hdg_edg(r, &hdg_edg);
    he_read_hdg_tri(r, &hdg_tri);

    MEMCPY(nh, nxt, q->nxt);
    MEMCPY(nh, flp, q->flp);
    MEMCPY(nh, ver, q->ver);
    MEMCPY(nh, tri, q->tri);
    MEMCPY(nh, edg, q->edg);
    MEMCPY(nv, hdg_ver, q->hdg_ver);
    MEMCPY(ne, hdg_edg, q->hdg_edg);
    MEMCPY(nt, hdg_tri, q->hdg_tri);

    q->magic = MAGIC;

    *pq = q;
    return CO_OK;
}

int
he_tri_ini(int nv, int nt, int *tri, T ** pq)
{
    int status;
    HeRead *read;

    he_read_tri_ini(nv, nt, tri, &read);
    status = he_ini(read, pq);
    he_read_fin(read);
    return status;
}

int
he_fin(T * q)
{
    if (q->magic != MAGIC)
        ERR(CO_MEMORY, "wrong fin() call");
    FREE(q->nxt);
    FREE(q->flp);
    FREE(q->ver);
    FREE(q->tri);
    FREE(q->edg);
    FREE(q->hdg_ver);
    FREE(q->hdg_edg);
    FREE(q->hdg_tri);

    FREE(q->E0);
    FREE(q->E1);
    FREE(q->T0);
    FREE(q->T1);
    FREE(q->T2);
    FREE(q->D0);
    FREE(q->D1);
    FREE(q->D2);
    FREE(q->D3);

    FREE(q);
    return CO_OK;
}

int
he_nv(T * q)
{
    return q->nv;
}

int
he_nt(T * q)
{
    return q->nt;
}

int
he_ne(T * q)
{
    return q->ne;
}

int
he_nh(T * q)
{
    return q->nh;
}

/* validate */
#define V(i, n) if (0 > (i) || (i) >= n)                        \
	ERR(CO_INDEX, "%s=%d is not in [0, %d)", #i, i, n)
int
he_nxt(T * q, int h)
{
    V(h, q->nh);
    return q->nxt[h];
}

int
he_flp(T * q, int h)
{
    int f;

    V(h, q->nh);
    if ((f = q->flp[h]) == -1)
        ERR(CO_INDEX, "no flip for %d", h);
    return f;
}

int
he_ver(T * q, int h)
{
    V(h, q->nh);
    return q->ver[h];
}

int
he_tri(T * q, int h)
{
    V(h, q->nh);
    return q->tri[h];
}

int
he_edg(T * q, int h)
{
    V(h, q->nh);
    return q->edg[h];
}

int
he_hdg_ver(T * q, int v)
{
    V(v, q->nv);
    return q->hdg_ver[v];
}

int
he_hdg_edg(T * q, int e)
{
    V(e, q->ne);
    return q->hdg_edg[e];
}

int
he_hdg_tri(T * q, int t)
{
    V(t, q->nt);
    return q->hdg_tri[t];
}

int
he_bnd(T * q, int h)
{
    V(h, q->nh);
    return q->flp[h] == -1;
}

int
he_ijk(T * q, int h, /**/ int *pi, int *pj, int *pk)
{
    int n, nn, i, j, k;

    n = he_nxt(q, h);
    nn = he_nxt(q, n);
    i = he_ver(q, h);
    j = he_ver(q, n);
    k = he_ver(q, nn);
    *pi = i;
    *pj = j;
    *pk = k;
    return CO_OK;
}

int
he_tri_ijk(T * q, int t, /**/ int *i, int *j, int *k)
{
    int h;

    h = he_hdg_tri(q, t);
    he_ijk(q, h, i, j, k);
    return CO_OK;
}

int
he_edg_ij(T * q, int e, /**/ int *pi, int *pj)
{
    int h, n, i, j;

    h = he_hdg_edg(q, e);
    n = he_nxt(q, h);

    i = he_ver(q, h);
    j = he_ver(q, n);

    *pi = i;
    *pj = j;
    return CO_OK;
}

int
he_dih_ijkl(T * q, int e, /**/ int *i, int *j, int *k, int *l)
{
    int h, n, nn, nnf;

    h = he_hdg_edg(q, e);
    n = he_nxt(q, h);
    nn = he_nxt(q, he_nxt(q, h));
    nnf = he_nxt(q, he_nxt(q, he_flp(q, h)));

    *j = he_ver(q, h);
    *k = he_ver(q, n);
    *i = he_ver(q, nn);
    *l = he_ver(q, nnf);

    return CO_OK;
}


static int
set_nxt(T * q, int h, int i)
{
    V(h, q->nh);
    V(i, q->nh);
    q->nxt[h] = i;
    return CO_OK;
}

static int
set_flp(T * q, int h, int i)
{
    V(h, q->nh);
    V(i, q->nh);
    q->flp[h] = i;
    return CO_OK;
}

static int
set_ver(T * q, int h, int i)
{
    V(h, q->nh);
    V(i, q->nv);
    q->ver[h] = i;
    return CO_OK;
}

static int
set_tri(T * q, int h, int i)
{
    V(h, q->nh);
    V(i, q->nt);
    q->tri[h] = i;
    return CO_OK;
}

static int
set_edg(T * q, int h, int i)
{
    V(h, q->nh);
    V(i, q->ne);
    q->edg[h] = i;
    return CO_OK;
}

static int
set_hdg_ver(T * q, int v, int i)
{
    V(v, q->nv);
    V(i, q->nh);
    q->hdg_ver[v] = i;
    return CO_OK;
}

static int
set_hdg_edg(T * q, int e, int i)
{
    V(e, q->ne);
    V(i, q->nh);
    q->hdg_edg[e] = i;
    return CO_OK;
}

static int
set_hdg_tri(T * q, int t, int i)
{
    V(t, q->nt);
    V(i, q->nh);
    q->hdg_tri[t] = i;
    return CO_OK;
}

#define  nxt(h)     he_nxt(q, (h))
#define  flp(h)     he_flp(q, (h))
#define  ver(h)     he_ver(q, (h))
#define  edg(h)     he_edg(q, (h))
#define  tri(h)     he_tri(q, (h))
#define  hdg_ver(v) he_hdg_ver(q, (v))
#define  hdg_edg(e) he_hdg_edg(q, (e))
#define  hdg_tri(t) he_hdg_tri(q, (t))
#define  s_nxt(h, i)     set_nxt(q, (h), (i))
#define  s_nxt3(t, i, j, k)			\
  s_hdg_tri(t, i),				\
  s_tri(i, t), s_tri(j, t), s_tri(k, t),	\
  s_nxt(i, j), s_nxt(j, k), s_nxt(k, i)
#define  s_flp(h, i)     set_flp(q, (h), (i)), set_flp(q, (i), (h))
#define  s_flp4(e, i, j, v, u) \
  s_hdg_edg(e, i), s_edg(i, e), s_edg(j, e), s_flp(i, j), s_ver(i, v), s_ver(j, u)
#define  s_ver(h, i)     set_ver(q, (h), (i))
#define  s_edg(h, i)     set_edg(q, (h), (i))
#define  s_tri(h, i)     set_tri(q, (h), (i))
#define  s_hdg_ver(v, i) set_hdg_ver(q, (v), (i))
#define  s_hdg_edg(e, i) set_hdg_edg(q, (e), (i))
#define  s_hdg_tri(t, i) set_hdg_tri(q, (t), (i))
int
he_edg_rotate(T * q, int e0)
{
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
    assert(h2 != h8);
    assert(h4 != h7);

    v0 = ver(h0);
    v1 = ver(h1);
    v2 = ver(h2);
    v3 = ver(h5);               /* sic */

    e1 = edg(h1);
    e2 = edg(h2);
    e3 = edg(h4);               /* sic */
    e4 = edg(h5);

    t0 = tri(h0);
    t1 = tri(h3);

    const int set[] = { h0, h1, h2, h3, h4, h5, h6, h7, h8, h9, END };
    if (!distinct(set)) {
        MSG("v: %d %d %d %d", v0, v1, v2, v3);
        MSG("e: %d %d %d %d", e0, e1, e2, e3, e4);
        MSG("t: %d %d", t0, t1);
        MSG("t: h2 = %d   h8 = %d", h2, h8);
        MSG("t: h4 = %d   h7 = %d", h4, h7);
        ERR(CO_INDEX, "h: %d %d %d %d %d %d %d %d %d %d",
            h0, h1, h2, h3, h4, h5, h6, h7, h8, h9);
    }

    assert(v0 == ver(h4));
    assert(v0 == ver(h7));
    assert(v1 == ver(h3));
    assert(v1 == ver(h9));
    assert(v2 == ver(h6));
    assert(v3 == ver(h8));

    assert(e0 == edg(h3));
    assert(e1 == edg(h6));
    assert(e2 == edg(h7));
    assert(e3 == edg(h8));
    assert(e4 == edg(h9));

    assert(t0 == tri(h1));
    assert(t0 == tri(h2));
    assert(t1 == tri(h4));
    assert(t1 == tri(h5));

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
    s_hdg_ver(v1, h5);
    s_hdg_ver(v2, h1);
    s_hdg_ver(v3, h4);

    s_hdg_edg(e0, h0);
    s_hdg_edg(e1, h5);
    s_hdg_edg(e2, h1);
    s_hdg_edg(e3, h2);
    s_hdg_edg(e4, h4);

    s_hdg_tri(t0, h0);
    s_hdg_tri(t1, h3);

    assert(v0 == ver(h2));
    assert(v0 == ver(h7));
    assert(v1 == ver(h5));
    assert(v1 == ver(h9));
    assert(v2 == ver(h6));
    assert(v3 == ver(h8));

    assert(e0 == edg(h3));
    assert(e1 == edg(h6));
    assert(e2 == edg(h7));
    assert(e3 == edg(h8));
    assert(e4 == edg(h9));

    assert(t0 == tri(h1));
    assert(t0 == tri(h2));
    assert(t1 == tri(h4));
    assert(t1 == tri(h5));

    assert(nxt(nxt(nxt(h0))) == h0);
    assert(nxt(nxt(nxt(h3))) == h3);

    assert(flp(flp(h0)) == h0);
    assert(flp(flp(h1)) == h1);
    assert(flp(flp(h2)) == h2);
    assert(flp(flp(h3)) == h3);
    assert(flp(flp(h4)) == h4);
    assert(flp(flp(h5)) == h5);
    assert(flp(flp(h6)) == h6);
    assert(flp(flp(h7)) == h7);
    assert(flp(flp(h8)) == h8);
    assert(flp(flp(h9)) == h9);

    return CO_OK;
}

int
he_edg_split(T * q, int e0)
{
    int nv, nt, ne, nh;
    int v0, v1, v2, v3;
    int t0, t1;
    int e1, e2, e3, e4;
    int h0, h1, h2, h3, h4, h5, h6, h7, h8, h9;

    int v4;
    int t2, t3;
    int e5, e6, e7;
    int h10, h11, h12, h13, h14, h15;

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
    v3 = ver(h5);               /* sic */

    e1 = edg(h1);
    e2 = edg(h2);
    e3 = edg(h4);               /* sic */
    e4 = edg(h5);

    t0 = tri(h0);
    t1 = tri(h3);

    nv = q->nv;
    nt = q->nt;
    ne = q->ne;
    nh = q->nh;

    v4 = nv++;
    t2 = nt++;
    t3 = nt++;
    e5 = ne++;
    e6 = ne++;
    e7 = ne++;
    h10 = nh++;
    h11 = nh++;
    h12 = nh++;
    h13 = nh++;
    h14 = nh++;
    h15 = nh++;
    q->nv = nv;
    q->nt = nt;
    q->ne = ne;
    q->nh = nh;

    /* */
    REALLOC(nh, &q->nxt);
    REALLOC(nh, &q->flp);
    REALLOC(nh, &q->ver);
    REALLOC(nh, &q->tri);
    REALLOC(nh, &q->edg);
    REALLOC(nv, &q->hdg_ver);
    REALLOC(ne, &q->hdg_edg);
    REALLOC(nt, &q->hdg_tri);
    REALLOC(ne, &q->E0);
    REALLOC(ne, &q->E1);
    REALLOC(nt, &q->T0);
    REALLOC(nt, &q->T1);
    REALLOC(nt, &q->T2);
    REALLOC(ne, &q->D0);
    REALLOC(ne, &q->D1);
    REALLOC(ne, &q->D2);
    REALLOC(ne, &q->D3);
    /* */

    s_nxt3(t0, h0, h1, h12);
    s_nxt3(t3, h15, h13, h2);
    s_nxt3(t2, h14, h4, h11);
    s_nxt3(t1, h3, h10, h5);

    s_flp4(e4, h9, h5, v1, v3);
    s_flp4(e6, h10, h11, v4, v3);
    s_flp4(e3, h4, h8, v0, v3);
    s_flp4(e0, h3, h0, v1, v4);
    s_flp4(e5, h14, h15, v4, v0);
    s_flp4(e1, h6, h1, v2, v1);
    s_flp4(e7, h12, h13, v2, v4);
    s_flp4(e2, h2, h7, v2, v0);

    s_hdg_ver(v0, h4);
    s_hdg_ver(v1, h1);
    s_hdg_ver(v2, h2);
    s_hdg_ver(v3, h5);
    s_hdg_ver(v4, h0);
    /* TODO */

    return CO_OK;
}

int
he_tri_split(T * q, int BDF)
{
int A;
int ABF;
int ABX;
int B;
int BCD;
int BCY;
int BYX;
int C;
int CDY;
int D;
int DEF;
int DEZ;
int DZY;
int E;
int EFZ;
int F;
int FAX;
int FXZ;
int X;
int XYZ;
int Y;
int Z;
int eAB;
int eBC;
int eBF;
int eBX;
int eCD;
int eCY;
int eDB;
int eDE;
int eDY;
int eEF;
int eEZ;
int eFA;
int eFD;
int eFZ;
int eXA;
int eXF;
int eXZ;
int eYB;
int eYX;
int eZD;
int eZY;
int hAB;
int hAF;
int hAX;
int hBA;
int hBC;
int hBD;
int hBF;
int hBX;
int hBY;
int hCB;
int hCD;
int hCY;
int hDB;
int hDC;
int hDE;
int hDF;
int hDY;
int hDZ;
int hED;
int hEF;
int hEZ;
int hFA;
int hFB;
int hFD;
int hFE;
int hFX;
int hFZ;
int hXA;
int hXB;
int hXF;
int hXY;
int hXZ;
int hYB;
int hYC;
int hYD;
int hYX;
int hYZ;
int hZD;
int hZE;
int hZF;
int hZX;
int hZY;
int nv;
int nt;
int ne;
int nh;
nv = q->nv;
nt = q->nt;
ne = q->ne;
nh = q->nh;
hBD = hdg_tri(BDF);
hDF = nxt(hBD);
hFB = nxt(hDF);
hBF = flp(hFB);
hFA = nxt(hBF);
hAB = nxt(hFA);
hBA = flp(hAB);
hAF = flp(hFA);
hFD = flp(hDF);
hDE = nxt(hFD);
hEF = nxt(hDE);
hFE = flp(hEF);
hED = flp(hDE);
hDB = flp(hBD);
hBC = nxt(hDB);
hCD = nxt(hBC);
hDC = flp(hCD);
hCB = flp(hBC);
/**/
A = ver(hAB);
B = ver(hBF);
C = ver(hCD);
D = ver(hDB);
E = ver(hEF);
F = ver(hFA);
eCD = edg(hCD);
eEF = edg(hEF);
eBC = edg(hBC);
eBF = edg(hBF);
eDB = edg(hDB);
eDE = edg(hDE);
eAB = edg(hAB);
eFA = edg(hFA);
eFD = edg(hFD);
BDF = tri(hBD);
DEF = tri(hDE);
BCD = tri(hBC);
ABF = tri(hAB);
Y = nv++;
Z = nv++;
X = nv++;
eDY = eDB;
eYX = eFD;
eXF = ne++;
eZD = ne++;
eFZ = ne++;
eCY = ne++;
eXZ = ne++;
eYB = ne++;
eEZ = ne++;
eZY = ne++;
eBX = ne++;
eXA = eBF;
EFZ = DEF;
FAX = BCD;
BYX = ABF;
DEZ = nt++;
DZY = nt++;
CDY = nt++;
FXZ = nt++;
BCY = nt++;
ABX = nt++;
XYZ = BDF;
hDZ = nh++;
hFX = hBF;
hYZ = nh++;
hZE = hDB;
hAX = nh++;
hFZ = hDF;
hZF = hFB;
hCY = hFD;
hXY = nh++;
hXZ = nh++;
hYB = nh++;
hYC = nh++;
hYD = nh++;
hZX = nh++;
hEZ = nh++;
hZY = nh++;
hBX = nh++;
hBY = nh++;
hXA = nh++;
hXB = nh++;
hDY = nh++;
hYX = nh++;
hXF = nh++;
hZD = hBD;
    q->nv = nv;
    q->nt = nt;
    q->ne = ne;
    q->nh = nh;
    /* */
    REALLOC(nh, &q->nxt);
    REALLOC(nh, &q->flp);
    REALLOC(nh, &q->ver);
    REALLOC(nh, &q->tri);
    REALLOC(nh, &q->edg);
    REALLOC(nv, &q->hdg_ver);
    REALLOC(ne, &q->hdg_edg);
    REALLOC(nt, &q->hdg_tri);
    REALLOC(ne, &q->E0);
    REALLOC(ne, &q->E1);
    REALLOC(nt, &q->T0);
    REALLOC(nt, &q->T1);
    REALLOC(nt, &q->T2);
    REALLOC(ne, &q->D0);
    REALLOC(ne, &q->D1);
    REALLOC(ne, &q->D2);
    REALLOC(ne, &q->D3);
    /* */
s_nxt(hCY, hYB);
s_nxt(hYC, hCD);
s_nxt(hAB, hBX);
s_nxt(hDY, hYC);
s_nxt(hYD, hDZ);
s_nxt(hZD, hDE);
s_nxt(hXF, hFA);
s_nxt(hDZ, hZY);
s_nxt(hFX, hXZ);
s_nxt(hBC, hCY);
s_nxt(hEZ, hZD);
s_nxt(hZE, hEF);
s_nxt(hFZ, hZE);
s_nxt(hZF, hFX);
s_nxt(hCD, hDY);
s_nxt(hFA, hAX);
s_nxt(hDE, hEZ);
s_nxt(hYX, hXB);
s_nxt(hXY, hYZ);
s_nxt(hEF, hFZ);
s_nxt(hXZ, hZF);
s_nxt(hZX, hXY);
s_nxt(hXA, hAB);
s_nxt(hAX, hXF);
s_nxt(hZY, hYD);
s_nxt(hYZ, hZX);
s_nxt(hBX, hXA);
s_nxt(hXB, hBY);
s_nxt(hYB, hBC);
s_nxt(hBY, hYX);
s_flp(hCY, hYC);
s_flp(hYC, hCY);
s_flp(hAB, hBA);
s_flp(hBA, hAB);
s_flp(hDY, hYD);
s_flp(hYD, hDY);
s_flp(hZD, hDZ);
s_flp(hDZ, hZD);
s_flp(hXF, hFX);
s_flp(hFX, hXF);
s_flp(hBC, hCB);
s_flp(hCB, hBC);
s_flp(hEZ, hZE);
s_flp(hZE, hEZ);
s_flp(hFZ, hZF);
s_flp(hZF, hFZ);
s_flp(hCD, hDC);
s_flp(hDC, hCD);
s_flp(hFA, hAF);
s_flp(hAF, hFA);
s_flp(hDE, hED);
s_flp(hED, hDE);
s_flp(hYX, hXY);
s_flp(hXY, hYX);
s_flp(hEF, hFE);
s_flp(hFE, hEF);
s_flp(hXZ, hZX);
s_flp(hZX, hXZ);
s_flp(hXA, hAX);
s_flp(hAX, hXA);
s_flp(hZY, hYZ);
s_flp(hYZ, hZY);
s_flp(hBX, hXB);
s_flp(hXB, hBX);
s_flp(hYB, hBY);
s_flp(hBY, hYB);
s_ver(hCY, C);
s_ver(hYC, Y);
s_ver(hAB, A);
s_ver(hDY, D);
s_ver(hYD, Y);
s_ver(hZD, Z);
s_ver(hXF, X);
s_ver(hDZ, D);
s_ver(hFX, F);
s_ver(hBC, B);
s_ver(hEZ, E);
s_ver(hZE, Z);
s_ver(hFZ, F);
s_ver(hZF, Z);
s_ver(hCD, C);
s_ver(hFA, F);
s_ver(hDE, D);
s_ver(hYX, Y);
s_ver(hXY, X);
s_ver(hEF, E);
s_ver(hXZ, X);
s_ver(hZX, Z);
s_ver(hXA, X);
s_ver(hAX, A);
s_ver(hZY, Z);
s_ver(hYZ, Y);
s_ver(hBX, B);
s_ver(hXB, X);
s_ver(hYB, Y);
s_ver(hBY, B);
s_tri(hCY, BCY);
s_tri(hYC, CDY);
s_tri(hAB, ABX);
s_tri(hDY, CDY);
s_tri(hYD, DZY);
s_tri(hZD, DEZ);
s_tri(hXF, FAX);
s_tri(hDZ, DZY);
s_tri(hFX, FXZ);
s_tri(hBC, BCY);
s_tri(hEZ, DEZ);
s_tri(hZE, EFZ);
s_tri(hFZ, EFZ);
s_tri(hZF, FXZ);
s_tri(hCD, CDY);
s_tri(hFA, FAX);
s_tri(hDE, DEZ);
s_tri(hYX, BYX);
s_tri(hXY, XYZ);
s_tri(hEF, EFZ);
s_tri(hXZ, FXZ);
s_tri(hZX, XYZ);
s_tri(hXA, ABX);
s_tri(hAX, FAX);
s_tri(hZY, DZY);
s_tri(hYZ, XYZ);
s_tri(hBX, ABX);
s_tri(hXB, BYX);
s_tri(hYB, BCY);
s_tri(hBY, BYX);
s_edg(hCY, eCY);
s_edg(hYC, eCY);
s_edg(hAB, eAB);
s_edg(hDY, eDY);
s_edg(hYD, eDY);
s_edg(hZD, eZD);
s_edg(hXF, eXF);
s_edg(hDZ, eZD);
s_edg(hFX, eXF);
s_edg(hBC, eBC);
s_edg(hEZ, eEZ);
s_edg(hZE, eEZ);
s_edg(hFZ, eFZ);
s_edg(hZF, eFZ);
s_edg(hCD, eCD);
s_edg(hFA, eFA);
s_edg(hDE, eDE);
s_edg(hYX, eYX);
s_edg(hXY, eYX);
s_edg(hEF, eEF);
s_edg(hXZ, eXZ);
s_edg(hZX, eXZ);
s_edg(hXA, eXA);
s_edg(hAX, eXA);
s_edg(hZY, eZY);
s_edg(hYZ, eZY);
s_edg(hBX, eBX);
s_edg(hXB, eBX);
s_edg(hYB, eYB);
s_edg(hBY, eYB);
s_hdg_ver(A, hAB);
s_hdg_ver(B, hBX);
s_hdg_ver(C, hCY);
s_hdg_ver(D, hDY);
s_hdg_ver(E, hEZ);
s_hdg_ver(F, hFZ);
s_hdg_ver(X, hXA);
s_hdg_ver(Y, hYB);
s_hdg_ver(Z, hZD);
s_hdg_edg(eZD, hZD);
s_hdg_edg(eXF, hXF);
s_hdg_edg(eBC, hBC);
s_hdg_edg(eEZ, hEZ);
s_hdg_edg(eFZ, hFZ);
s_hdg_edg(eCD, hCD);
s_hdg_edg(eFA, hFA);
s_hdg_edg(eDE, hDE);
s_hdg_edg(eYX, hYX);
s_hdg_edg(eEF, hEF);
s_hdg_edg(eXZ, hXZ);
s_hdg_edg(eXA, hXA);
s_hdg_edg(eZY, hZY);
s_hdg_edg(eBX, hBX);
s_hdg_edg(eYB, hYB);
s_hdg_edg(eCY, hCY);
s_hdg_edg(eAB, hAB);
s_hdg_edg(eDY, hDY);
s_hdg_tri(BYX, hBY);
s_hdg_tri(BCY, hBC);
s_hdg_tri(DEZ, hDE);
s_hdg_tri(FXZ, hFX);
s_hdg_tri(XYZ, hXY);
s_hdg_tri(FAX, hFA);
s_hdg_tri(ABX, hAB);
s_hdg_tri(CDY, hCD);
s_hdg_tri(EFZ, hEF);
s_hdg_tri(DZY, hDZ);
 return CO_OK;  
}


int
he_E(T * he, int **pE0, int **pE1)
{
    int ne, e, *E0, *E1;
    int h, n;

    E0 = he->E0;
    E1 = he->E1;
    ne = he_ne(he);

    for (e = 0; e < ne; e++) {
        h = he_hdg_edg(he, e);
        if (he_bnd(he, h)) {
            E0[e] = E1[e] = -1;
        } else {
            n = he_nxt(he, h);
            E0[e] = he_ver(he, h);
            E1[e] = he_ver(he, n);
        }
    }
    *pE0 = E0;
    *pE1 = E1;
    return CO_OK;
}

int
he_T(T * he, int **pT0, int **pT1, int **pT2)
{
    int nt, t, *T0, *T1, *T2;
    int h, n, nn;

    T0 = he->T0;
    T1 = he->T1;
    T2 = he->T2;
    nt = he_nt(he);

    for (t = 0; t < nt; t++) {
        h = he_hdg_tri(he, t);
        n = he_nxt(he, h);
        nn = he_nxt(he, n);
        T0[t] = he_ver(he, h);
        T1[t] = he_ver(he, n);
        T2[t] = he_ver(he, nn);
    }

    *pT0 = T0;
    *pT1 = T1;
    *pT2 = T2;
    return CO_OK;
}

int
he_D(T * he, int **pD0, int **pD1, int **pD2, int **pD3)
{
    int ne, e, *D0, *D1, *D2, *D3;
    int h, n, nn, nnf;

    D0 = he->D0;
    D1 = he->D1;
    D2 = he->D2;
    D3 = he->D3;
    ne = he_ne(he);

    for (e = 0; e < ne; e++) {
        h = he_hdg_edg(he, e);
        if (he_bnd(he, h)) {
            D0[e] = D1[e] = D2[e] = D3[e] = -1;
        } else {
            h = he_hdg_edg(he, e);
            n = he_nxt(he, h);
            nn = he_nxt(he, he_nxt(he, h));
            nnf = he_nxt(he, he_nxt(he, he_flp(he, h)));
            D1[e] = he_ver(he, h);
            D2[e] = he_ver(he, n);
            D0[e] = he_ver(he, nn);
            D3[e] = he_ver(he, nnf);
        }
    }
    *pD0 = D0;
    *pD1 = D1;
    *pD2 = D2;
    *pD3 = D3;
    return CO_OK;
}

int
he_bnd_ver(T * he, int v)
{
    int h, h0, n, f, rank;

    h = he_hdg_ver(he, v);
    rank = 0;
    h0 = h = he_nxt(he, h);
    for (;;) {
        if (rank >= RANK_MAX)
            ERR(CO_INDEX, "v=%d, rank=%d >= RANK_MAX=%d", v, rank,
                RANK_MAX);
        rank++;
        n = he_nxt(he, h);
        if (he_bnd(he, n))
            return 1;
        f = he_flp(he, n);
        h = he_nxt(he, f);
        if (h == h0)
            return 0;
    }
}

int
he_ring(T * he, int v, int *prank, int **pring)
{
    int h, h0, n, f, i, rank;
    int *ring;

    ring = he->ring;

    if (v >= he->nv)
        ERR(CO_INDEX, "v=%d >= q->nv=%d", v, he->nv);
    h = he_hdg_ver(he, v);
    if (he_bnd(he, h))
        ERR(CO_INDEX, "call ring for boundary v = %d, h = %d", v, h);

    rank = 0;
    h0 = h = he_nxt(he, h);
    for (;;) {
        if (rank >= RANK_MAX)
            ERR(CO_INDEX, "v=%d, rank=%d >= RANK_MAX=%d", v, rank,
                RANK_MAX);
        i = he_ver(he, h);
        ring[rank++] = i;
        n = he_nxt(he, h);
        f = he_flp(he, n);
        h = he_nxt(he, f);
        if (h == h0)
            break;
    }
    ring[rank] = -1;

    *prank = rank;
    *pring = he->ring;
    return CO_OK;
}
