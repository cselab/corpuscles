#include <stdio.h>
#include "real.h"

#include "co/bbox2.h"
#include "co/err.h"
#include "co/skel.h"
#include "co/memory.h"
#include "co/predicate.h"
#include "co/list/edg1.h"
#include "co/vec2.h"

#include "co/polygon.h"

enum {
  X, Y, Z
};

#define T Polygon
struct T {
  Bbox2 *bbox;
  Skel *skel;
  Edg1List *list;
  const real *x, *y;
};

int
polygon_ini(real lo, real hi, real size, T ** pq)
{
  T *q;

  MALLOC(1, &q);
  bbox2_ini(&q->bbox);
  edg1list_ini(lo, hi, size, &q->list);
  predicate_ini();
  *pq = q;
  return CO_OK;
}

int
polygon_fin(T * q)
{
  bbox2_fin(q->bbox);
  edg1list_fin(q->list);
  FREE(q);
  return CO_OK;
}

int
polygon_update(T * q, Skel * skel, const real * x, const real * y)
{
  int n;

  q->skel = skel;
  q->x = x;
  q->y = y;
  n = skel_nv(skel);
  bbox2_update(q->bbox, n, x, y);
  edg1list_push(q->list, skel, x);
  return CO_OK;
}

#define max(a, b) ( (a) > (b) ? (a) : (b) )
int
polygon_inside(T * q, real u, real v)
{
  int n, t, i, j, m;
  Skel *skel;
  Bbox2 *bbox;
  const real *x, *y;
  real a[3], b[3], d[3], e[3];
  real ym, eps;

  eps = 1e-10;
  skel = q->skel;
  x = q->x;
  y = q->y;
  bbox = q->bbox;
  vec2_ini(u, v, /**/ d);
  ym = bbox2_yhi(bbox);
  vec2_ini(u, max(ym, v) + eps, /**/ e);
  n = skel_ne(skel);
  for (t = m = 0; t < n; t++) {
    skel_edg_ij(skel, t, &i, &j);
    vec2_get(i, x, y, a);
    vec2_get(j, x, y, b);
    m += predicate_ray2(d, e, a, b);
  }
  return m % 2;
}

#define max(a, b) ( (a) > (b) ? (a) : (b) )
int
polygon_inside_fast(T * q, real u, real v)
{
  int t, i, j, m;
  Skel *skel;
  Bbox2 *bbox;
  const real *x, *y;
  real a[3], b[3], d[3], e[3];
  real ym, eps;
  int *edgs;

  eps = 1e-10;
  skel = q->skel;
  x = q->x;
  y = q->y;
  bbox = q->bbox;
  if (!bbox2_inside(bbox, u, v))
    return 0;
  vec2_ini(u, v, /**/ d);
  ym = bbox2_yhi(bbox);
  vec2_ini(u, max(ym, v) + eps, /**/ e);
  edg1list_edgs(q->list, u, &edgs);
  m = 0;
  while ((t = *edgs++) != -1) {
    skel_edg_ij(skel, t, &i, &j);
    vec2_get(i, x, y, a);
    vec2_get(j, x, y, b);
    m += predicate_ray2(d, e, a, b);
  }
  return m % 2;
}
