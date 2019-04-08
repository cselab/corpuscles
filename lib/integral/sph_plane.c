#include <stdio.h>
#include <tgmath.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/integral/sph_plane.h"

typedef struct AlgIntegration AlgIntegration;
enum {QNG, GAUSS15, GAUSS21, GAUSS31, GAUSS61};
int alg_integration_ini(int, AlgIntegration**);
int alg_integration_fin(AlgIntegration*);
int alg_integration_apply(AlgIntegration*, real, real, real (*)(real, void*), void*, /**/ real*);

enum {TYPE = GAUSS31};
#define T SphPlane

struct T
{
	AlgIntegration *ie, *ih, *ig;
	real R, d;
	real p0, p, t;
	void *param;
	real (*E)(real, real, real, real, void*);
};

int
sph_plane_ini(real R, real (*E)(real x, real y, real z, real d, void*), void *param, T **pq)
{
	T *q;

	MALLOC(1, &q);
	q->R = R;
	alg_integration_ini(TYPE, &q->ie);
	alg_integration_ini(TYPE, &q->ih);
	alg_integration_ini(TYPE, &q->ig);
	*pq = q;
	return CO_OK;
}

int
sph_plane_fin(T *q)
{
	alg_integration_fin(q->ie);
	alg_integration_fin(q->ih);
	alg_integration_fin(q->ig);
	FREE(q);
}

int
sph_plane_apply(T *q, real d, /**/ real *result)
{
	return CO_OK;
}