#include <stdio.h>
#include <real.h>

#include <co/memory.h>
#include <co/vec.h>
#include <co/y.h>
#include <co/surface.h>
#include <co/punto.h>

#define FMT CO_REAL_OUT

enum {
  X, Y, Z
};

int
main(int argc, const char **argv)
{
  Surface *surface;
  He *he;
  real *x, *y, *z, *u, *v, *w, *color;

  int m, n, i, j, k, l;
  real lo, hi, dx, x0, y0, z0;
  real clo[3] = { -2, -2, -2 };
  real chi[3] = { 2, 2, 2 };
  real size = 0.2;

  m = 50;
  lo = -1;
  hi = 1;
  n = m * m * m;
  MALLOC(n, &u);
  MALLOC(n, &v);
  MALLOC(n, &w);
  CALLOC(n, &color);

  y_inif(stdin, &he, &x, &y, &z);
  surface_ini(clo, chi, size, &surface);
  surface_update(surface, he, x, y, z);

  dx = (hi - lo) / (m - 1);
  for (l = i = 0; i < m; i++) {
    x0 = lo + dx * i;
    for (j = 0; j < m; j++) {
      y0 = lo + dx * j;
      for (k = 0; k < m; k++) {
        z0 = lo + dx * k;
        u[l] = x0;
        v[l] = y0;
        w[l] = z0;
        color[l] = surface_inside_fast(surface, u[l], v[l], w[l]);
        l++;
      }
    }
  }

  const real *q[] = {
    u, v, w, color, NULL
  };
  punto_fwrite(n, q, stdout);

  FREE(u);
  FREE(v);
  FREE(w);
  FREE(color);
  y_fin(he, x, y, z);
  surface_fin(surface);
  return 0;
}

/*

make clean lint
9 time ./grid < /u/1.off > q
punto -c p
wc -l p
wc -l q

gnuplot -p <<'!'
plot "q" u 1:4, "p" u 1:4
!

diff p q

*/
