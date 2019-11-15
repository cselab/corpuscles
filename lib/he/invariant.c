#include <stdlib.h>
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/he/invariant.h"

int
he_invariant_nxt(He *he)
{
    int h, nh, n, nn, nnn;
    nh = he_nh(he);

    for (h = 0; h < nh; h++) {
	n  = he_nxt(he, h);
	nn = he_nxt(he, n);
	nnn = he_nxt(he, nn);
	if (nnn != h)
	    ERR(CO_NUM, "nnn=%d != h=%d", nnn, h);
    }
    return CO_OK;
}

int
he_invariant_flp(He *he)
{
    int nh, h, f, ff;
    nh = he_nh(he);

    for (h = 0; h < nh; h++) {
	if (he_bnd(he, h)) continue;
	f  = he_flp(he, h);
	ff = he_flp(he, f);
	if (ff != h)
	    ERR(CO_NUM, "ff=%d != h=%d", ff, h);
    }
    return CO_OK;
}

int
he_invariant_ver(He *he)
{
    int nv, v, vv, h;
    nv = he_nv(he);

    for (v = 0; v < nv; v++) {
	h = he_hdg_ver(he, v);
	vv = he_ver(he, h);
	if (vv != v)
	    ERR(CO_NUM, "vv=%d != v=%d", vv, v);
    }
    return CO_OK;
}

int
he_invariant_tri(He *he)
{
    int nt, t, tt, h;
    nt = he_nt(he);

    for (t = 0; t < nt; t++) {
	h = he_hdg_tri(he, t);
	tt = he_tri(he, h);
	if (tt != t)
	    ERR(CO_NUM, "tt=%d != t=%d", tt, t);
    }
    return CO_OK;
}

int
he_invariant_edg(He *he)
{
    int ne, e, ee, h;
    ne = he_ne(he);

    for (e = 0; e < ne; e++) {
	h = he_hdg_edg(he, e);
	ee = he_edg(he, h);
	if (ee != e)
	    ERR(CO_NUM, "ee=%d != e=%d", ee, e);
    }
    return CO_OK;
}

int
he_invariant_edg_visit(He *he)
{
#define CNT_EDG(h)				\
    do {					\
	e = he_edg(he, h);			\
	cnt[e] += he_bnd(he, h) ? 2 : 1;	\
    } while (0)

    int ne, nt, t, h, hh, hhh, e, *cnt;
    ne = he_ne(he);
    nt = he_nt(he);
    CALLOC(ne, &cnt);
    for (t = 0; t < nt; t++) {
	h = he_hdg_tri(he, t);
	hh = he_nxt(he, h);
	hhh = he_nxt(he, hh);
	CNT_EDG(h);
	CNT_EDG(hh);
	CNT_EDG(hhh);
    }
    for (e = 0; e < ne; e++)
	if (cnt[e] != 2)
	    ERR(CO_NUM, "cnt[%d]=%d != 2", e, cnt[e]);
    FREE(cnt);
    return CO_OK;
}
