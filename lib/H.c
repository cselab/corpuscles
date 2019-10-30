#include <tgmath.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/laplace.h"
#include "co/normal.h"
#include "co/H.h"

#define T H
#define MAGIC (12345)

struct T {
  int nv;
  Laplace *laplace;
  real *nx, *ny, *nz;
  real *hh;
  int magic;
};

int
H_ini(He * he, /**/ T ** pq)
{
  T *q;
  int status;
  int nv;

  MALLOC(1, &q);
  nv = he_nv(he);
  status = laplace_ini(he, &q->laplace);
  if (status != CO_OK)
    ERR(CO_NUM, "laplace_ini failed");

  MALLOC(nv, &q->hh);
  MALLOC(nv, &q->nx);
  MALLOC(nv, &q->ny);
  MALLOC(nv, &q->nz);

  q->magic = MAGIC;
  q->nv = nv;
  *pq = q;
  return status;
}

int
H_fin(T * q)
{
  if (q->magic != MAGIC)
    ERR(CO_MEMORY, "H_fin call");
  laplace_fin(q->laplace);
  FREE(q->hh);
  FREE(q->nx);
  FREE(q->ny);
  FREE(q->nz);
  FREE(q);
  return CO_OK;
}

int
H_apply(T * q, He * he, const real * x, const real * y, const real * z,
        /**/ real ** pH, real ** parea)
{
  int nv, i;
  real *lx, *ly, *lz, *area;
  real *nx, *ny, *nz;
  real *hh;

  if (q->magic != MAGIC)
    ERR(CO_MEMORY, "H_apply call");

  nv = he_nv(he);
  hh = q->hh;
  nx = q->nx;
  ny = q->ny;
  nz = q->nz;

  laplace_apply(q->laplace, he, x, y, z, &lx, &ly, &lz, &area);
  normal_mwa(he, x, y, z, /**/ nx, ny, nz);

  for (i = 0; i < nv; i++)
    hh[i] = lx[i] * nx[i] + ly[i] * ny[i] + lz[i] * nz[i];

  *pH = q->hh;
  *parea = area;
  return CO_OK;
}
