#include <stdio.h>
#include <tgmath.h>

#include <real.h>
#include <co/arc.h>
#include <co/err.h>
#include <co/memory.h>
#include <real.h>

#define T Skel
typedef struct T T;
int skel_close_ini(int nv, T**);
int skel_open_ini(int nv, T**);
int skel_read(FILE*, /**/ real**, real**, T**);
int skel_fin(T*);
int skel_xy_fin(real*, real*, T*);

int skel_write(T*, const real*, const real*, FILE*);
int skel_nv(T*);
int skel_ne(T*);
int skel_bnd(T*, int);
int skel_edg_ij(T*, int, /**/  int*, int*);
#undef T

#define T Skel
struct T
{
	int nv, Close;
};

int
skel_close_ini(int nv, T **pq)
{
	T *q;
	
	if (nv < 0)
		ERR(CO_INDEX, "nv=%d < 0", nv);
	MALLOC(1, &q);
	q->nv = nv;
	q->Close = 1;
	*pq = q;
	return CO_OK;
}

int
skel_open_ini(int nv, T **pq)
{
	T *q;	
	if (nv < 0)
		ERR(CO_INDEX, "nv=%d < 0", nv);
	MALLOC(1, &q);
	q->nv = nv;
	q->Close = 0;
	*pq = q;
	return CO_OK;
}

int
skel_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

int
skel_xy_fin(real *x, real *y, T *q)
{
	FREE(x);
	FREE(y);
	FREE(q);
	return CO_OK;
}

int
skel_write(T *q, const real *x, const real *y, FILE *f)
{
	 
}

#define PI (3.141592653589793)
int
main(void)
{
	
}
