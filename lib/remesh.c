#include <math.h>
#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/remesh.h"
#include "co/memory.h"

static int tri_near (He * he, int t, int *i, int *j, int *k);

int
remesh_tri_split (int nmax, int *tri, int n, He * he, real ** px, real ** py,
		  real ** pz)
{
  int a;
  int b;
  int c;
  int i;
  int ia;
  int ib;
  int ic;
  int j;
  int *killed;
  int nt;
  int nv;
  int t;
  real u[3];
  real v[3];
  real va[3];
  real vb[3];
  real vc[3];
  real w[3];
  real *x;
  real *y;
  real *z;

  x = *px;
  y = *py;
  z = *pz;
  nt = he_nt (he);
  i = j = 0;
  CALLOC (nt, &killed);
  while (i < nmax && j < n)
    {
      t = tri[i];
      if (t < 0 || t >= nt)
	ERR (CO_INDEX, "tri[%d]=%d is not in [0, %d)", i, t, nt);
      if (!killed[t] && tri_near (he, t, &a, &b, &c) == 0)
	{
	  he_tri_ijk (he, t, &ia, &ib, &ic);
	  vec_get3 (ia, ib, ic, x, y, z, va, vb, vc);
	  if (he_tri_split (he, t) != CO_OK)
	    ERR (CO_INDEX, "he_tri_split failed, t = %d", t);
	  if (a < nt)
	    killed[a] = 1;
	  if (b < nt)
	    killed[b] = 1;
	  if (c < nt)
	    killed[c] = 1;
	  nv = he_nv(he);
	  REALLOC (nv, &x);	/* TODO */
	  REALLOC (nv, &y);
	  REALLOC (nv, &z);
	  tri_edg_center (va, vb, vc, u, v, w);
	  vec_set (v, nv - 1, x, y, z);
	  vec_set (u, nv - 2, x, y, z);
	  vec_set (w, nv - 3, x, y, z);
	  j++;
	}
      i++;
    }
  *px = x;
  *py = y;
  *pz = z;
  FREE (killed);
  return CO_OK;
}

static int
tri_near (He * he, int t, int *pi, int *pj, int *pk)
{
  int i;
  int j;
  int k;

  i = he_hdg_tri (he, t);
  j = he_nxt (he, i);
  k = he_nxt (he, j);
  if (he_bnd (he, i) || he_bnd (he, j) || he_bnd (he, k))
    return 1;
  i = he_flp (he, i);
  j = he_flp (he, j);
  k = he_flp (he, k);
  *pi = he_tri (he, i);
  *pj = he_tri (he, j);
  *pk = he_tri (he, k);
  return 0;
}
