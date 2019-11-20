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

#define  nxt(h)     (he_nxt(q, (h)))
#define  flp(h)     (he_flp(q, (h)))
#define  ver(h)     (he_ver(q, (h)))
#define  edg(h)     (he_edg(q, (h)))
#define  tri(h)     (he_tri(q, (h)))
#define  hdg_ver(v) (he_hdg_ver(q, (v)))
#define  hdg_edg(e) (he_hdg_edg(q, (e)))
#define  hdg_tri(t) (he_hdg_tri(q, (t)))
#define  bnd_ver(v) (he_bnd_ver(q, (v)))
#define  bnd(h) (he_bnd(q, (h)))
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

#define DEL_VER(i) he_swap_ver(q, (i), --nv)
#define DEL_EDG(i) he_swap_edg(q, (i), --ne)
#define DEL_TRI(i) he_swap_tri(q, (i), --nt)
#define DEL_HDG(i) he_swap_hdg(q, (i), --nh)

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
he_ver_delete5(T * q, int X)
{
    int nv, nt, ne, nh;
int A;
int ABC;
int ABX;
int ACE;
int AEX;
int B;
int BCX;
int C;
int CDE;
int CDX;
int D;
int DEX;
int E;
int eAB;
int eAC;
int eAE;
int eAX;
int eBC;
int eBX;
int eCD;
int eCE;
int eCX;
int eDE;
int eDX;
int eEX;
int hAB;
int hAC;
int hAE;
int hAX;
int hBA;
int hBC;
int hBX;
int hCA;
int hCB;
int hCD;
int hCE;
int hCX;
int hDC;
int hDE;
int hDX;
int hEA;
int hEC;
int hED;
int hEX;
int hXA;
int hXB;
int hXC;
int hXD;
int hXE;

nv = q->nv;
nt = q->nt;
ne = q->ne;
nh = q->nh;

hXA = hdg_ver(X);
hAB = nxt(hXA);
hBX = nxt(hAB);
hXB = flp(hBX);
hBC = nxt(hXB);
hCX = nxt(hBC);
hXC = flp(hCX);
hCD = nxt(hXC);
hDX = nxt(hCD);
hXD = flp(hDX);
hDE = nxt(hXD);
hEX = nxt(hDE);
hXE = flp(hEX);
hEA = nxt(hXE);
hAX = nxt(hEA);
hAE = flp(hEA);
hED = flp(hDE);
hDC = flp(hCD);
hCB = flp(hBC);
hBA = flp(hAB);
/**/
A = ver(hAB);
B = ver(hBX);
C = ver(hCX);
D = ver(hDX);
E = ver(hEX);
X = ver(hXA);
eBC = edg(hBC);
eAE = edg(hEA);
eCD = edg(hCD);
eDE = edg(hDE);
eAX = edg(hXA);
eBX = edg(hBX);
eCX = edg(hCX);
eDX = edg(hDX);
eEX = edg(hEX);
eAB = edg(hAB);
AEX = tri(hEA);
DEX = tri(hDE);
BCX = tri(hBC);
CDX = tri(hCD);
ABX = tri(hAB);
eAC = eAX;
eCE = eBX;
ABC = AEX;
CDE = DEX;
ACE = BCX;
hCA = hXD;
hAC = hDX;
hCE = hXE;
hEC = hEX;

DEL_VER(X);
DEL_EDG(eCX);
DEL_EDG(eDX);
DEL_EDG(eEX);
DEL_TRI(CDX);
DEL_TRI(ABX);
DEL_HDG(hAX);
DEL_HDG(hXA);
DEL_HDG(hXB);
DEL_HDG(hBX);
DEL_HDG(hXC);
DEL_HDG(hCX);

s_nxt(hAB, hBC);
s_nxt(hAC, hCE);
s_nxt(hCA, hAB);
s_nxt(hBC, hCA);
s_nxt(hEA, hAC);
s_nxt(hCD, hDE);
s_nxt(hEC, hCD);
s_nxt(hCE, hEA);
s_nxt(hDE, hEC);
s_flp(hBA, hAB);
s_flp(hAB, hBA);
s_flp(hAC, hCA);
s_flp(hCA, hAC);
s_flp(hCB, hBC);
s_flp(hBC, hCB);
s_flp(hAE, hEA);
s_flp(hEA, hAE);
s_flp(hDC, hCD);
s_flp(hCD, hDC);
s_flp(hEC, hCE);
s_flp(hCE, hEC);
s_flp(hED, hDE);
s_flp(hDE, hED);
s_ver(hAB, A);
s_ver(hAC, A);
s_ver(hCA, C);
s_ver(hBC, B);
s_ver(hEA, E);
s_ver(hCD, C);
s_ver(hEC, E);
s_ver(hCE, C);
s_ver(hDE, D);
s_tri(hAB, ABC);
s_tri(hAC, ACE);
s_tri(hCA, ABC);
s_tri(hBC, ABC);
s_tri(hEA, ACE);
s_tri(hCD, CDE);
s_tri(hEC, CDE);
s_tri(hCE, ACE);
s_tri(hDE, CDE);
s_edg(hAB, eAB);
s_edg(hAC, eAC);
s_edg(hCA, eAC);
s_edg(hBC, eBC);
s_edg(hEA, eAE);
s_edg(hCD, eCD);
s_edg(hEC, eCE);
s_edg(hCE, eCE);
s_edg(hDE, eDE);
s_hdg_ver(A, hAB);
s_hdg_ver(B, hBC);
s_hdg_ver(C, hCA);
s_hdg_ver(D, hDE);
s_hdg_ver(E, hEA);
s_hdg_edg(eAC, hCA);
s_hdg_edg(eBC, hBC);
s_hdg_edg(eAE, hEA);
s_hdg_edg(eCD, hCD);
s_hdg_edg(eCE, hCE);
s_hdg_edg(eDE, hDE);
s_hdg_edg(eAB, hAB);
s_hdg_tri(ABC, hAB);
s_hdg_tri(CDE, hCD);
s_hdg_tri(ACE, hAC);
    q->nv = nv;
    q->nt = nt;
    q->ne = ne;
    q->nh = nh;
    return CO_OK;
}

int
he_ver_delete6(T * q, int X)
{
    int nv, nt, ne, nh;
    int A;
int ABC;
int ABX;
int ACF;
int AFX;
int B;
int BCX;
int C;
int CDF;
int CDX;
int D;
int DEF;
int DEX;
int E;
int eAB;
int eAC;
int eAF;
int eAX;
int eBC;
int eBX;
int eCD;
int eCF;
int eCX;
int eDE;
int eDF;
int eDX;
int eEF;
int eEX;
int eFX;
int EFX;
int F;
int hAB;
int hAC;
int hAF;
int hAX;
int hBA;
int hBC;
int hBX;
int hCA;
int hCB;
int hCD;
int hCF;
int hCX;
int hDC;
int hDE;
int hDF;
int hDX;
int hED;
int hEF;
int hEX;
int hFA;
int hFC;
int hFD;
int hFE;
int hFX;
int hXA;
int hXB;
int hXC;
int hXD;
int hXE;
int hXF;

nv = q->nv;
nt = q->nt;
ne = q->ne;
nh = q->nh;
hXA = hdg_ver(X);
hAB = nxt(hXA);
hBX = nxt(hAB);
hXB = flp(hBX);
hBC = nxt(hXB);
hCX = nxt(hBC);
hXC = flp(hCX);
hCD = nxt(hXC);
hDX = nxt(hCD);
hXD = flp(hDX);
hDE = nxt(hXD);
hEX = nxt(hDE);
hXE = flp(hEX);
hEF = nxt(hXE);
hFX = nxt(hEF);
hXF = flp(hFX);
hFA = nxt(hXF);
hAX = nxt(hFA);
hAF = flp(hFA);
hFE = flp(hEF);
hED = flp(hDE);
hDC = flp(hCD);
hCB = flp(hBC);
hBA = flp(hAB);
/**/
A = ver(hAB);
B = ver(hBX);
C = ver(hCX);
D = ver(hDX);
E = ver(hEX);
F = ver(hFX);
X = ver(hXA);
eFX = edg(hFX);
eBC = edg(hBC);
eAF = edg(hFA);
eCD = edg(hCD);
eDE = edg(hDE);
eEF = edg(hEF);
eAX = edg(hXA);
eBX = edg(hBX);
eCX = edg(hCX);
eDX = edg(hDX);
eEX = edg(hEX);
eAB = edg(hAB);
AFX = tri(hFA);
DEX = tri(hDE);
BCX = tri(hBC);
EFX = tri(hEF);
CDX = tri(hCD);
ABX = tri(hAB);
eAC = eFX;
eCF = eAX;
eDF = eBX;
ABC = AFX;
CDF = DEX;
ACF = BCX;
DEF = EFX;
hCA = hXD;
hAC = hDX;
hCF = hXE;
hFC = hEX;
hDF = hXF;
hFD = hFX;
DEL_VER(X);
DEL_EDG(eCX);
DEL_EDG(eDX);
DEL_EDG(eEX);
DEL_TRI(CDX);
DEL_TRI(ABX);
DEL_HDG(hAX);
DEL_HDG(hXA);
DEL_HDG(hXB);
DEL_HDG(hBX);
DEL_HDG(hXC);
DEL_HDG(hCX);
s_nxt(hAB, hBC);
s_nxt(hAC, hCF);
s_nxt(hCA, hAB);
s_nxt(hBC, hCA);
s_nxt(hCD, hDF);
s_nxt(hFA, hAC);
s_nxt(hDE, hEF);
s_nxt(hFC, hCD);
s_nxt(hCF, hFA);
s_nxt(hFD, hDE);
s_nxt(hDF, hFC);
s_nxt(hEF, hFD);
s_flp(hBA, hAB);
s_flp(hAB, hBA);
s_flp(hAC, hCA);
s_flp(hCA, hAC);
s_flp(hCB, hBC);
s_flp(hBC, hCB);
s_flp(hDC, hCD);
s_flp(hCD, hDC);
s_flp(hAF, hFA);
s_flp(hFA, hAF);
s_flp(hED, hDE);
s_flp(hDE, hED);
s_flp(hFC, hCF);
s_flp(hCF, hFC);
s_flp(hFD, hDF);
s_flp(hDF, hFD);
s_flp(hFE, hEF);
s_flp(hEF, hFE);
s_ver(hAB, A);
s_ver(hAC, A);
s_ver(hCA, C);
s_ver(hBC, B);
s_ver(hCD, C);
s_ver(hFA, F);
s_ver(hDE, D);
s_ver(hFC, F);
s_ver(hCF, C);
s_ver(hFD, F);
s_ver(hDF, D);
s_ver(hEF, E);
s_tri(hAB, ABC);
s_tri(hAC, ACF);
s_tri(hCA, ABC);
s_tri(hBC, ABC);
s_tri(hCD, CDF);
s_tri(hFA, ACF);
s_tri(hDE, DEF);
s_tri(hFC, CDF);
s_tri(hCF, ACF);
s_tri(hFD, DEF);
s_tri(hDF, CDF);
s_tri(hEF, DEF);
s_edg(hAB, eAB);
s_edg(hAC, eAC);
s_edg(hCA, eAC);
s_edg(hBC, eBC);
s_edg(hCD, eCD);
s_edg(hFA, eAF);
s_edg(hDE, eDE);
s_edg(hFC, eCF);
s_edg(hCF, eCF);
s_edg(hFD, eDF);
s_edg(hDF, eDF);
s_edg(hEF, eEF);
s_hdg_ver(A, hAB);
s_hdg_ver(B, hBC);
s_hdg_ver(C, hCA);
s_hdg_ver(D, hDF);
s_hdg_ver(E, hEF);
s_hdg_ver(F, hFA);
s_hdg_edg(eAC, hCA);
s_hdg_edg(eBC, hBC);
s_hdg_edg(eCD, hCD);
s_hdg_edg(eAF, hFA);
s_hdg_edg(eDE, hDE);
s_hdg_edg(eCF, hCF);
s_hdg_edg(eDF, hDF);
s_hdg_edg(eEF, hEF);
s_hdg_edg(eAB, hAB);
s_hdg_tri(ABC, hAB);
s_hdg_tri(CDF, hCD);
s_hdg_tri(ACF, hAC);
s_hdg_tri(DEF, hDE);
    q->nv = nv;
    q->nt = nt;
    q->ne = ne;
    q->nh = nh;
    return CO_OK;
}

int
he_ver_delete7(T * q, int X)
{
    int nv, nt, ne, nh;
int A;
int ABC;
int ABX;
int ACG;
int AGX;
int B;
int BCX;
int C;
int CDG;
int CDX;
int D;
int DEF;
int DEX;
int DFG;
int E;
int eAB;
int eAC;
int eAG;
int eAX;
int eBC;
int eBX;
int eCD;
int eCG;
int eCX;
int eDE;
int eDF;
int eDG;
int eDX;
int eEF;
int eEX;
int eFG;
int eFX;
int EFX;
int eGX;
int F;
int FGX;
int G;
int hAB;
int hAC;
int hAG;
int hAX;
int hBA;
int hBC;
int hBX;
int hCA;
int hCB;
int hCD;
int hCG;
int hCX;
int hDC;
int hDE;
int hDF;
int hDG;
int hDX;
int hED;
int hEF;
int hEX;
int hFD;
int hFE;
int hFG;
int hFX;
int hGA;
int hGC;
int hGD;
int hGF;
int hGX;
int hXA;
int hXB;
int hXC;
int hXD;
int hXE;
int hXF;
int hXG;

nv = q->nv;
nt = q->nt;
ne = q->ne;
nh = q->nh;

hXA = hdg_ver(X);
hAB = nxt(hXA);
hBX = nxt(hAB);
hXB = flp(hBX);
hBC = nxt(hXB);
hCX = nxt(hBC);
hXC = flp(hCX);
hCD = nxt(hXC);
hDX = nxt(hCD);
hXD = flp(hDX);
hDE = nxt(hXD);
hEX = nxt(hDE);
hXE = flp(hEX);
hEF = nxt(hXE);
hFX = nxt(hEF);
hXF = flp(hFX);
hFG = nxt(hXF);
hGX = nxt(hFG);
hXG = flp(hGX);
hGA = nxt(hXG);
hAX = nxt(hGA);
hAG = flp(hGA);
hGF = flp(hFG);
hFE = flp(hEF);
hED = flp(hDE);
hDC = flp(hCD);
hCB = flp(hBC);
hBA = flp(hAB);
/**/
A = ver(hAB);
B = ver(hBX);
C = ver(hCX);
D = ver(hDX);
E = ver(hEX);
F = ver(hFX);
G = ver(hGX);
X = ver(hXA);
eFX = edg(hFX);
eGX = edg(hGX);
eBC = edg(hBC);
eCD = edg(hCD);
eAG = edg(hGA);
eDE = edg(hDE);
eEF = edg(hEF);
eAX = edg(hXA);
eFG = edg(hFG);
eBX = edg(hBX);
eCX = edg(hCX);
eDX = edg(hDX);
eEX = edg(hEX);
eAB = edg(hAB);
FGX = tri(hFG);
AGX = tri(hGA);
DEX = tri(hDE);
BCX = tri(hBC);
EFX = tri(hEF);
CDX = tri(hCD);
ABX = tri(hAB);

DEL_VER(X);
DEL_EDG(eCX);
DEL_EDG(eDX);
DEL_EDG(eEX);
DEL_TRI(CDX);
DEL_TRI(ABX);
DEL_HDG(hAX);
DEL_HDG(hXA);
DEL_HDG(hXB);
DEL_HDG(hBX);
DEL_HDG(hXC);
DEL_HDG(hCX);

eAC = eFX;
eCG = eGX;
eDF = eAX;
eDG = eBX;
DFG = FGX;
ABC = AGX;
CDG = DEX;
ACG = BCX;
DEF = EFX;
hCA = hXD;
hAC = hDX;
hCG = hXE;
hGC = hEX;
hDF = hXF;
hFD = hFX;
hDG = hXG;
hGD = hGX;
s_nxt(hAB, hBC);
s_nxt(hAC, hCG);
s_nxt(hCA, hAB);
s_nxt(hBC, hCA);
s_nxt(hCD, hDG);
s_nxt(hGA, hAC);
s_nxt(hDE, hEF);
s_nxt(hFD, hDE);
s_nxt(hDF, hFG);
s_nxt(hGC, hCD);
s_nxt(hCG, hGA);
s_nxt(hEF, hFD);
s_nxt(hGD, hDF);
s_nxt(hDG, hGC);
s_nxt(hFG, hGD);
s_flp(hBA, hAB);
s_flp(hAB, hBA);
s_flp(hAC, hCA);
s_flp(hCA, hAC);
s_flp(hCB, hBC);
s_flp(hBC, hCB);
s_flp(hDC, hCD);
s_flp(hCD, hDC);
s_flp(hAG, hGA);
s_flp(hGA, hAG);
s_flp(hED, hDE);
s_flp(hDE, hED);
s_flp(hFD, hDF);
s_flp(hDF, hFD);
s_flp(hGC, hCG);
s_flp(hCG, hGC);
s_flp(hFE, hEF);
s_flp(hEF, hFE);
s_flp(hGD, hDG);
s_flp(hDG, hGD);
s_flp(hGF, hFG);
s_flp(hFG, hGF);
s_ver(hAB, A);
s_ver(hAC, A);
s_ver(hCA, C);
s_ver(hBC, B);
s_ver(hCD, C);
s_ver(hGA, G);
s_ver(hDE, D);
s_ver(hFD, F);
s_ver(hDF, D);
s_ver(hGC, G);
s_ver(hCG, C);
s_ver(hEF, E);
s_ver(hGD, G);
s_ver(hDG, D);
s_ver(hFG, F);
s_tri(hAB, ABC);
s_tri(hAC, ACG);
s_tri(hCA, ABC);
s_tri(hBC, ABC);
s_tri(hCD, CDG);
s_tri(hGA, ACG);
s_tri(hDE, DEF);
s_tri(hFD, DEF);
s_tri(hDF, DFG);
s_tri(hGC, CDG);
s_tri(hCG, ACG);
s_tri(hEF, DEF);
s_tri(hGD, DFG);
s_tri(hDG, CDG);
s_tri(hFG, DFG);
s_edg(hAB, eAB);
s_edg(hAC, eAC);
s_edg(hCA, eAC);
s_edg(hBC, eBC);
s_edg(hCD, eCD);
s_edg(hGA, eAG);
s_edg(hDE, eDE);
s_edg(hFD, eDF);
s_edg(hDF, eDF);
s_edg(hGC, eCG);
s_edg(hCG, eCG);
s_edg(hEF, eEF);
s_edg(hGD, eDG);
s_edg(hDG, eDG);
s_edg(hFG, eFG);
s_hdg_ver(A, hAB);
s_hdg_ver(B, hBC);
s_hdg_ver(C, hCA);
s_hdg_ver(D, hDG);
s_hdg_ver(E, hEF);
s_hdg_ver(F, hFG);
s_hdg_ver(G, hGA);
s_hdg_edg(eAC, hCA);
s_hdg_edg(eBC, hBC);
s_hdg_edg(eCD, hCD);
s_hdg_edg(eAG, hGA);
s_hdg_edg(eDE, hDE);
s_hdg_edg(eDF, hDF);
s_hdg_edg(eCG, hCG);
s_hdg_edg(eEF, hEF);
s_hdg_edg(eDG, hDG);
s_hdg_edg(eFG, hFG);
s_hdg_edg(eAB, hAB);
s_hdg_tri(DFG, hDF);
s_hdg_tri(ABC, hAB);
s_hdg_tri(CDG, hCD);
s_hdg_tri(ACG, hAC);
s_hdg_tri(DEF, hDE);
    q->nv = nv;
    q->nt = nt;
    q->ne = ne;
    q->nh = nh;
    return CO_OK;
}

int
he_ver_delete4(T * q, int X)
{
        int nv, nt, ne, nh;
int A;
int ABD;
int ABX;
int ADX;
int B;
int BCD;
int BCX;
int C;
int CDX;
int D;
int eAB;
int eAD;
int eAX;
int eBC;
int eBD;
int eBX;
int eCD;
int eCX;
int eDX;
int hAB;
int hAD;
int hAX;
int hBA;
int hBC;
int hBD;
int hBX;
int hCB;
int hCD;
int hCX;
int hDA;
int hDB;
int hDC;
int hDX;
int hXA;
int hXB;
int hXC;
int hXD;

nv = q->nv;
nt = q->nt;
ne = q->ne;
nh = q->nh;

hXA = hdg_ver(X);
hAB = nxt(hXA);
hBX = nxt(hAB);
hXB = flp(hBX);
hBC = nxt(hXB);
hCX = nxt(hBC);
hXC = flp(hCX);
hCD = nxt(hXC);
hDX = nxt(hCD);
hXD = flp(hDX);
hDA = nxt(hXD);
hAX = nxt(hDA);
hAD = flp(hDA);
hDC = flp(hCD);
hCB = flp(hBC);
hBA = flp(hAB);
A = ver(hAB);
B = ver(hBX);
C = ver(hCX);
D = ver(hDX);
eAD = edg(hDA);
eBC = edg(hBC);
eCD = edg(hCD);
eAX = edg(hXA);
eBX = edg(hBX);
eCX = edg(hCX);
eDX = edg(hDX);
eAB = edg(hAB);
ADX = tri(hDA);
BCX = tri(hBC);
CDX = tri(hCD);
ABX = tri(hAB);

DEL_VER(X);
DEL_EDG(eBX);
DEL_EDG(eCX);
DEL_EDG(eDX);
DEL_TRI(CDX);
DEL_TRI(ABX);
DEL_HDG(hAX);
DEL_HDG(hXA);
DEL_HDG(hXB);
DEL_HDG(hBX);
DEL_HDG(hXC);
DEL_HDG(hCX);

/**/
eBD = eAX;
ABD = ADX;
BCD = BCX;
hBD = hXD;
hDB = hDX;
s_nxt(hAB, hBD);
s_nxt(hBC, hCD);
s_nxt(hDA, hAB);
s_nxt(hDB, hBC);
s_nxt(hBD, hDA);
s_nxt(hCD, hDB);
s_flp(hBA, hAB);
s_flp(hAB, hBA);
s_flp(hCB, hBC);
s_flp(hBC, hCB);
s_flp(hAD, hDA);
s_flp(hDA, hAD);
s_flp(hDB, hBD);
s_flp(hBD, hDB);
s_flp(hDC, hCD);
s_flp(hCD, hDC);
s_ver(hAB, A);
s_ver(hBC, B);
s_ver(hDA, D);
s_ver(hDB, D);
s_ver(hBD, B);
s_ver(hCD, C);
s_tri(hAB, ABD);
s_tri(hBC, BCD);
s_tri(hDA, ABD);
s_tri(hDB, BCD);
s_tri(hBD, ABD);
s_tri(hCD, BCD);
s_edg(hAB, eAB);
s_edg(hBC, eBC);
s_edg(hDA, eAD);
s_edg(hDB, eBD);
s_edg(hBD, eBD);
s_edg(hCD, eCD);
s_hdg_ver(A, hAB);
s_hdg_ver(B, hBD);
s_hdg_ver(C, hCD);
s_hdg_ver(D, hDA);
s_hdg_edg(eBC, hBC);
s_hdg_edg(eAD, hDA);
s_hdg_edg(eBD, hBD);
s_hdg_edg(eCD, hCD);
s_hdg_edg(eAB, hAB);
s_hdg_tri(ABD, hAB);
s_hdg_tri(BCD, hBC);
    q->nv = nv;
    q->nt = nt;
    q->ne = ne;
    q->nh = nh;
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
     /**/ A = ver(hAB);
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
he_swap_ver(T * q, int i, int j)
{
    int hi, hj, nv, h;

    if (bnd_ver(i))
	ERR(CO_INDEX, "bnd_ver(%d)", i);
    if (bnd_ver(j))
	ERR(CO_INDEX, "bnd_ver(%d)", j);
    if (i == j)
	return CO_OK;
    nv = he_nv(q);
    if (i >= nv)
	ERR(CO_INDEX, "i=%d >= nv=%d", i, nv);
    if (j >= nv)
	ERR(CO_INDEX, "j=%d >= nv=%d", j, nv);
    hi = hdg_ver(i);
    hj = hdg_ver(j);
    h = hi;
    do {
	assert(ver(h) == i);
	s_ver(h, j);
	h = flp(nxt(nxt(h)));
    } while (h != hi);
    h = hj;
    do {
	assert(ver(h) == j);
	s_ver(h, i);
	h = flp(nxt(nxt(h)));
    } while (h != hj);
    s_hdg_ver(i, hj);
    s_hdg_ver(j, hi);
    return CO_OK;
}

int
he_swap_tri(T * q, int i, int j)
{
    int hi, hj, nt, h;

    if (i == j)
	return CO_OK;
    nt = he_nt(q);
    if (i >= nt)
	ERR(CO_INDEX, "i=%d >= nt=%d", i, nt);
    if (j >= nt)
	ERR(CO_INDEX, "j=%d >= nt=%d", j, nt);

    hi = hdg_tri(i);
    hj = hdg_tri(j);
    h = hi;
    do {
	assert(tri(h) == i);
	s_tri(h, j);
	h = nxt(h);
    } while (h != hi);
    h = hj;
    do {
	assert(tri(h) == j);
	s_tri(h, i);
	h = nxt(h);
    } while (h != hj);
    s_hdg_tri(i, hj);
    s_hdg_tri(j, hi);
    return CO_OK;
}

int
he_swap_hdg(T * q, int i, int j)
{
#define MAP(v, i, j) if ((v) == (j)) (v) = (i)
    int nh;
    int ni, nni, fi, vi, ti, ei;
    int nj, nnj, fj, vj, tj, ej;
    if (i == j)
	return CO_OK;
    nh = he_nh(q);
    if (i >= nh)
	ERR(CO_INDEX, "i=%d >= nh=%d", i, nh);
    if (j >= nh)
	ERR(CO_INDEX, "j=%d >= nh=%d", j, nh);
    ni = nxt(i);
    nni = nxt(ni);
    fi = flp(i);
    MAP(ni, i, j);
    MAP(nni, i, j);
    MAP(fi, i, j);
    vi = ver(i);
    ti = tri(i);
    ei = edg(i);

    nj = nxt(j);
    nnj = nxt(nj);
    fj = flp(j);
    MAP(nj, j, i);
    MAP(nnj, j, i);
    MAP(fj, j, i);
    vj = ver(j);
    tj = tri(j);
    ej = edg(j);

    s_nxt(i, nj); s_nxt(nnj, i);
    s_flp(i, fj); s_flp(fj, i);
    
    s_ver(i, vj);
    s_tri(i, tj);
    s_edg(i, ej);
    s_hdg_ver(vj, i);
    s_hdg_tri(tj, i);
    s_hdg_edg(ej, i);

    s_nxt(j, ni); s_nxt(nni, j);
    s_flp(j, fi); s_flp(fi, j);
    s_ver(j, vi);
    s_tri(j, ti);
    s_edg(j, ei);
    s_hdg_ver(vi, j);
    s_hdg_tri(ti, j);
    s_hdg_edg(ei, j);

    return CO_OK;
}

int
he_swap_edg(T * q, int i, int j)
{
    int hi, hj, ne, fi, fj;

    if (i == j)
	return CO_OK;
    ne = he_ne(q);
    if (i >= ne)
	ERR(CO_INDEX, "i=%d >= ne=%d", i, ne);
    if (j >= ne)
	ERR(CO_INDEX, "j=%d >= ne=%d", j, ne);
    hi = hdg_edg(i);
    hj = hdg_edg(j);
    if (bnd(hi))
	ERR(CO_INDEX, "bnd(%d)", i);
    if (bnd(hj))
	ERR(CO_INDEX, "bnd(%d)", j);
    fi = flp(hi);
    fj = flp(hj);
    assert(edg(fi) == i);
    assert(edg(fj) == j);
    s_edg(fi, j);
    s_edg(hi, j);
    s_hdg_edg(j, hi);

    s_edg(hj, i);
    s_edg(fj, i);
    s_hdg_edg(i, hj);

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
