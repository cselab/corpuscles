#include <stdio.h>
#include <stdlib.h>
#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/vtk.h"
#include "co/generation.h"

#define T Generation
struct T {
    int *g;
    int *mate;                  /* tri to hdg */
    int NT;                     /* alloc sizes */
    int NV;
    int *mbit;                  /* mate bit-flag */
};
typedef struct T T;
static int tri_mate(T *, int, He *);
static int bit_set(int generation, int *mbit);
static int bit_clear(int generation, int *mbit);
static int bit_get(int mbit, int generation, int *ans);

int
generation_ini(He * he, T ** pq)
{
    int i;
    int n;
    T *q;

    MALLOC(1, &q);
    n = he_nt(he);
    MALLOC(n, &q->g);
    MALLOC(n, &q->mate);
    MALLOC(n, &q->mbit);
    q->NT = n;
    q->NV = he_nv(he);
    for (i = 0; i < n; i++) {
	q->g[i] = 0;
	q->mbit[i] = 0;
    }
    *pq = q;
    return CO_OK;
}

int
generation_fin(T * q)
{
    FREE(q->g);
    FREE(q->mate);
    FREE(q->mbit);
    FREE(q);
    return CO_OK;
}

static int
realloc0(T * q, int nv, int nt, real ** x, real ** y, real ** z)
{
    if (nt > q->NT) {
	q->NT *= 2;
	REALLOC(q->NT, &q->g);
	REALLOC(q->NT, &q->mate);
	REALLOC(q->NT, &q->mbit);
    }
    if (nv > q->NV) {
	q->NV *= 2;
	REALLOC(q->NV, x);
	REALLOC(q->NV, y);
	REALLOC(q->NV, z);
    }
    return CO_OK;
}

static int
center(int t, He * he, const real * x, const real * y, const real * z,
       real d[3])
{
    int i;
    int j;
    int k;
    real a[3];
    real b[3];
    real c[3];

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    return tri_center(a, b, c, d);
}

static int
swap0(T * q, int t, He * he, int *status)
{
    int e;
    int m;                      /* triangle */

    if (q->g[t] % 2 == 0)
	ERR(CO_INDEX, "cannot swap even triangle (t=%d, g[t]=%d)", t,
	    q->g[t]);
    e = he_edg(he, q->mate[t]);
    m = tri_mate(q, t, he);
    if (q->g[t] != q->g[m]) {
	*status = 1;
	return CO_OK;
    }
    if (he_edg_rotate(he, e) != CO_OK)
	ERR(CO_INDEX, "he_edg_rotate failed (e=%d)", e);
    q->g[t] += 1;
    q->g[m] += 1;
    *status = 0;
    return CO_OK;
}

static int
split0(T * q, int t, He * he, real ** x, real ** y, real ** z, int *pu,
       int *pv, int *pw)
{
    int a;
    int b;
    int Bit;
    int c;
    int Generation;
    int Mate;
    int nt;
    int nv;
    int u;
    int v;
    int w;
    real Center[3];

    if (q->g[t] % 2 != 0)
	ERR(CO_INDEX, "cannot split odd triangle (t=%d)", t);
    a = he_hdg_tri(he, t);      /* half edges */
    b = he_nxt(he, a);
    c = he_nxt(he, b);
    Generation = q->g[t];
    Bit = q->mbit[t];
    Mate = q->mate[t];
    center(t, he, *x, *y, *z, Center);
    if (he_tri_split3(he, t) != CO_OK)
	ERR(CO_INDEX, "he_tri_split3 failed (t=%d)", t);
    nv = he_nv(he);
    nt = he_nt(he);
    realloc0(q, nv, nt, x, y, z);
    vec_set(Center, nv - 1, *x, *y, *z);
    u = he_tri(he, a);          /* new triangles */
    v = he_tri(he, b);
    w = he_tri(he, c);
    q->mate[u] = a;
    q->mate[v] = b;
    q->mate[w] = c;
    q->g[u] = Generation + 1;
    q->g[v] = Generation + 1;
    q->g[w] = Generation + 1;
    q->mbit[u] = Bit;
    q->mbit[v] = Bit;
    q->mbit[w] = Bit;
    if (Generation > 0) {
	if (a == Mate)
	    bit_set(Generation, &q->mbit[u]);
	if (b == Mate)
	    bit_set(Generation, &q->mbit[v]);
	if (c == Mate)
	    bit_set(Generation, &q->mbit[w]);
    }
    *pu = u;
    *pv = v;
    *pw = w;
    return CO_OK;
}

static int
split(T * q, int t, He * he, real ** x, real ** y, real ** z)
{
    int a;
    int b;
    int c;
    int status;

    if (split0(q, t, he, x, y, z, &a, &b, &c) != CO_OK)
	ERR(CO_INDEX, "split0 failed (t = %d)", t);
    swap0(q, a, he, &status);
    swap0(q, b, he, &status);
    swap0(q, c, he, &status);
    return CO_OK;
}

static int
tri_mate(T * q, int t, He * he)
{
    int m;

    m = q->mate[t];
    if (he_bnd(he, m))
	ERR(CO_INDEX, "cannot swap on the boundary (t=%d, m=%d)", t, m);
    m = he_flp(he, m);
    m = he_tri(he, m);
    return m;
}

int
generation_refine(T * q, int t, He * he, real ** x, real ** y, real ** z)
{
    int m;

    if (q->g[t] % 2 == 0) {
	return split(q, t, he, x, y, z);
    } else {
	m = tri_mate(q, t, he);
	if (q->g[m] == q->g[t] - 2) {
	    generation_refine(q, m, he, x, y, z);
	    m = tri_mate(q, t, he);
	}
	return generation_refine(q, m, he, x, y, z);
    }
}

int
generation_invariant(T * q, He * he)
{
    int ne;
    int h;
    int f;
    int i;
    int j;
    int e;

    ne = he_ne(he);
    for (e = 0; e < ne; e++) {
	h = he_hdg_edg(he, e);
	f = he_flp(he, h);
	i = he_tri(he, h);
	j = he_tri(he, f);
	if (q->g[i] + 2 < q->g[j])
	    ERR(CO_INDEX, "ij: %d %d", i, j);
	if (q->g[j] + 2 < q->g[i])
	    ERR(CO_INDEX, "ij: %d %d", i, j);
    }
    return CO_OK;
}

int
generation_write(T * q, He * he, const real * x, const real * y,
		 const real * z, FILE * file)
{
    int status;
    int h;
    int n;
    int t;
    int v;
    int *mver;

    n = he_nt(he);
    MALLOC(n, &mver);
    for (t = 0; t < n; t++)
	mver[t] = -1;
    for (t = 0; t < n; t++)
	if (q->g[t] % 2 == 1) {
	    h = q->mate[t];
	    h = he_nxt(he, h);
	    h = he_nxt(he, h);
	    v = he_ver(he, h);
	    mver[t] = v;
	}
    const char *names[] = { "g", "mbit", "mver", NULL };
    const int *scalars[] = { q->g, q->mbit, mver, NULL };
    status = vtk_tri_int_write(he, x, y, z, scalars, names, file);
    if (status != CO_OK)
	ERR(CO_IO, "vtk_tri_int_write faield");
    FREE(mver);
    return CO_OK;
}

int
generation_color(T *q, He *he, real *color)
{
    int i;
    int n;

    n = he_nt(he);
    for (i = 0; i < n; i++)
	color[i] = q->g[i];
    return CO_OK;
}

int
generation_read(FILE * file, He ** phe, real ** x, real ** y, real ** z,
		T ** pq)
{
    He *he;
    int *mver;
    int nt;
    int t;
    int cnt;
    int h;
    int v;
    T *q;

    MALLOC(1, &q);
    const char *names[] = { "g", "mbit", "mver", NULL };
    int **scalars[] = { &q->g, &q->mbit, &mver, NULL };
    if (vtk_tri_int_read(file, names, &he, x, y, z, scalars) != CO_OK)
	ERR(CO_IO, "vtk_tri_int_read failed");
    nt = he_nt(he);
    MALLOC(nt, &q->mate);
    for (t = 0; t < nt; t++)
	if (q->g[t] % 2 == 1) {
	    v = mver[t];
	    h = he_hdg_tri(he, t);
	    for (cnt = 0;; cnt++) {
		if (he_ver(he, h) == v)
		    break;
		h = he_nxt(he, h);
		if (cnt > 3)
		    ERR(CO_IO, "v = %d, h = %d, t = %d", v, h, t);
	    }
	    h = he_nxt(he, h);
	    q->mate[t] = h;
	}
    q->NT = nt;
    q->NV = he_nv(he);
    *phe = he;
    *pq = q;
    FREE(mver);
    return CO_OK;
}

static int
bit_set(int n, int *mbit)
{
    if (n % 2 == 1)
	ERR(CO_INDEX, "generation cannot be odd");
    if (n > 50)
	ERR(CO_INDEX, "generation is too big (n = %d)", n);
    n /= 2;
    (*mbit) |= 1 << n;
    return CO_OK;
}

static int
bit_clear(int n, int *mbit)
{
    if (n % 2 == 1)
	ERR(CO_INDEX, "generation cannot be odd");
    if (n > 50)
	ERR(CO_INDEX, "generation is too big (n = %d)", n);
    n /= 2;
    (*mbit) &= ~(1 << n);
    return CO_OK;
}

static int
bit_get(int mbit, int n, int *ans)
{
    if (n % 2 == 1)
	ERR(CO_INDEX, "generation cannot be odd");
    if (n > 50)
	ERR(CO_INDEX, "generation is too big (n = %d)", n);
    n /= 2;
    *ans = (mbit >> n) & 1;
}
