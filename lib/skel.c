#include <stdio.h>
#include <string.h>
#include <tgmath.h>
#include "real.h"
#include "co/arc.h"
#include "co/array.h"
#include "co/colormap.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/skel.h"
#include "co/util.h"
#include "inc/def.h"

#define FMT CO_REAL_IN
#define OUT CO_REAL_OUT

#define PI (3.141592653589793)
enum {
	SIZE = MAX_STRING_SIZE
};

#define T Skel
struct T {
	int nv, Close;
};

int
skel_close_ini(int nv, T ** pq)
{
	T *q;

	if (nv <= 0)
		ERR(CO_INDEX, "nv=%d <= 0", nv);
	MALLOC(1, &q);
	q->nv = nv;
	q->Close = 1;
	*pq = q;
	return CO_OK;
}

int
skel_open_ini(int nv, T ** pq)
{
	T *q;

	if (nv <= 0)
		ERR(CO_INDEX, "nv=%d <= 0", nv);
	MALLOC(1, &q);
	q->nv = nv;
	q->Close = 0;
	*pq = q;
	return CO_OK;
}

int
skel_circle_ini(int nv, /**/ real * length, real ** px, real ** py,
				T ** pq)
{
	real *x, *y;
	real lo, hi, d, p;
	int i;

	if (nv <= 1)
		ERR(CO_INDEX, "nv=%d <= 1", nv);
	MALLOC(nv, &x);
	MALLOC(nv, &y);
	lo = 0;
	hi = 2 * PI;
	d = (lo - hi) / nv;
	for (i = 0; i < nv; i++) {
		p = lo + d * i;
		x[i] = cos(p);
		y[i] = sin(p);
	}
	skel_close_ini(nv, pq);
	*length = 2 * PI;
	*px = x;
	*py = y;
	return CO_OK;
}

#define NXT \
	if (util_comment_fgets(line, f) == NULL)  \
		ERR(CO_IO, "unexpected EOF")
int
skel_read(FILE * f, real ** px, real ** py, T ** pq)
{
	real *x, *y;
	int i, nv, nv0, np;
	char line[SIZE];

	NXT;
	if (!util_eq(line, "SKEL"))
		ERR(CO_IO, "expect 'SKEL', got '%s'", line);
	NXT;
	if (sscanf(line, "%d %d", &nv, &np) != 2)
		ERR(CO_IO, "fail to parse '%s'", line);
	if (np != 1)
		ERR(CO_IO, "expect one polyline, got %d", np);
	MALLOC(nv, &x);
	MALLOC(nv, &y);
	for (i = 0; i < nv; i++) {
		NXT;
		if (sscanf(line, FMT " " FMT, &x[i], &y[i]) != 2)
			ERR(CO_IO, "fail to parse '%s'", line);
	}
	if (fscanf(f, "%d", &nv0) != 1)
		ERR(CO_IO, "fail to read polyline");
	*px = x;
	*py = y;
	if (nv0 == nv)
		return skel_open_ini(nv, pq);
	else if (nv0 == nv + 1)
		return skel_close_ini(nv, pq);
	else
		ERR(CO_IO, "nv0=%d, nv=%d", nv0, nv);
}

#undef NXT

int
skel_fin(T * q)
{
	FREE(q);
	return CO_OK;
}

int
skel_xy_fin(real * x, real * y, T * q)
{
	FREE(x);
	FREE(y);
	FREE(q);
	return CO_OK;
}

int
skel_write(T * q, const real * x, const real * y, FILE * f)
{
	int nv, Close, i, npolylines = 1;
	real z = 0;

	Close = q->Close;
	nv = q->nv;
	if (fputs("SKEL\n", f) == EOF)
		ERR(CO_IO, "fail to write");
	fprintf(f, "%d %d\n", nv, npolylines);
	for (i = 0; i < nv; i++)
		fprintf(f, OUT " " OUT " " OUT "\n", x[i], y[i], z);
	fprintf(f, "%d", Close ? nv + 1 : nv);
	for (i = 0; i < nv; i++)
		fprintf(f, " %d", i);
	fprintf(f, Close ? " 0\n" : "\n");
	return CO_OK;
}

int
skel_punto_write(T * q, const real * x, const real * y, FILE * f)
{
	int nv, i, status;
	real z = 0;

	nv = q->nv;
	for (i = 0; i < nv; i++) {
		status = fprintf(f, OUT " " OUT " " OUT "\n", x[i], y[i], z);
		if (status < 0)
			ERR(CO_IO, "fail to prin");
	}
	return CO_OK;
}

int
skel_off_write(int nv, const real * x, const real * y, FILE * f)
{
	int nt, ne, i;
	real z = 0;
	float red = 0, green = 1, blue = 0;

	nt = nv;
	ne = 0;
	if (fputs("OFF\n", f) == EOF)
		ERR(CO_IO, "fail to write");
	fprintf(f, "%d %d %d\n", nv, nt, ne);
	for (i = 0; i < nv; i++)
		fprintf(f, OUT " " OUT " " OUT "\n", x[i], y[i], z);
	for (i = 0; i < nv; i++)
		fprintf(f, "1 %d %g %g %g\n", i, red, green, blue);
	return CO_OK;
}

int
skel_vtk_write(T * q, const real * x, const real * y, FILE * f)
{
	int nv, ne, e, i, j;
	real z = 0;

	nv = skel_nv(q);
	ne = skel_ne(q);
	if (fputs("# vtk DataFile Version 2.0\n", f) == EOF)
		ERR(CO_IO, "fail to write");
	fputs("skel file\n", f);
	fputs("ASCII\n", f);
	fputs("DATASET POLYDATA\n", f);
	fprintf(f, "POINTS %d double\n", nv);
	for (i = 0; i < nv; i++)
		fprintf(f, OUT " " OUT " " OUT "\n", x[i], y[i], z);
	fprintf(f, "POLYGONS %d %d\n", ne, (2 + 1) * ne);
	for (e = 0; e < ne; e++) {
		skel_edg_ij(q, e, &i, &j);
		fprintf(f, "2 %d %d\n", i, j);
	}
	return CO_OK;
}

int
skel_edg_write(T * q, const real * x, const real * y, FILE * f)
{
	int nv, nt, ne, e, i, j;
	real z = 0;
	float red = 0, green = 1, blue = 0;

	nv = skel_nv(q);
	nt = skel_ne(q);
	ne = 0;
	if (fputs("OFF\n", f) == EOF)
		ERR(CO_IO, "fail to write");
	fprintf(f, "%d %d %d\n", nv, nt, ne);
	for (i = 0; i < nv; i++)
		fprintf(f, OUT " " OUT " " OUT "\n", x[i], y[i], z);
	for (e = 0; e < nt; e++) {
		skel_edg_ij(q, e, &i, &j);
		fprintf(f, "2 %d %d %g %g %g\n", i, j, red, green, blue);
	}
	return CO_OK;
}

int
skel_edg_lh_write(T * q, const real * x, const real * y, real lo, real hi,
				  const real * a, FILE * f)
{
	int nv, nt, ne, e, i, j;
	real z = 0;
	float red, green, blue;

	nv = skel_nv(q);
	nt = skel_ne(q);
	ne = 0;
	if (fputs("OFF\n", f) == EOF)
		ERR(CO_IO, "fail to write");
	fprintf(f, "%d %d %d\n", nv, nt, ne);
	for (i = 0; i < nv; i++)
		fprintf(f, OUT " " OUT " " OUT "\n", x[i], y[i], z);
	for (e = 0; e < nt; e++) {
		skel_edg_ij(q, e, &i, &j);
		colormap(a[e], lo, hi, &red, &green, &blue);
		fprintf(f, "2 %d %d %g %g %g\n", i, j, red, green, blue);
	}
	return CO_OK;
}

int
skel_edg_color_write(T * q, const real * x, const real * y, const real * a,
					 FILE * f)
{
	int ne;
	real lo, hi;

	ne = skel_ne(q);
	lo = array_min(ne, a);
	hi = array_max(ne, a);
	return skel_edg_lh_write(q, x, y, lo, hi, a, f);
}

int
skel_nv(T * q)
{
	return q->nv;
}

int
skel_ne(T * q)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	return Close ? nv : nv - 1;
}

int
skel_bnd(T * q, int i)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	if (i < 0)
		ERR(CO_INDEX, "i=%d < 0", i);
	if (i >= nv)
		ERR(CO_INDEX, "i=%d >= nv=%d", i, nv);
	if (Close)
		return 0;
	else
		return i == 0 || i == nv - 1;
}

int
skel_close(T * q)
{
	return q->Close;
}

int
skel_edg_ij(T * q, int e, int *i, int *j)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	if (e < 0)
		ERR(CO_INDEX, "e=%d < 0", e);
	if (Close && e >= nv)
		ERR(CO_INDEX, "e=%d >= nv=%d", e, nv);
	if (!Close && e >= nv - 1)
		ERR(CO_INDEX, "e=%d >= nv=%d", e, nv);
	if (Close) {
		*i = e;
		*j = e + 1;
		if (*j == nv)
			*j = 0;
	} else {
		*i = e;
		*j = e + 1;
	}
	return CO_OK;
}

int
skel_ver_ijk(T * q, int v, int *i, int *j, int *k)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	if (v < 0)
		ERR(CO_INDEX, "v=%d < 0", v);
	if (Close && v >= nv)
		ERR(CO_INDEX, "v=%d >= nv=%d", v, nv);
	if (!Close && v == nv - 1)
		ERR(CO_INDEX, "v=%d == nv - 1=%d", v, nv - 1);
	if (!Close && v == 0)
		ERR(CO_INDEX, "v=%d == 0", v);
	*i = v - 1;
	*j = v;
	*k = v + 1;
	if (Close) {
		if (*i == -1)
			*i = nv - 1;
		if (*k == nv)
			*k = 0;
	}
	return CO_OK;
}
