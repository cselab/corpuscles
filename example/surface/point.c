#include <stdio.h>
#include <real.h>

#include <co/vec.h>
#include <co/y.h>
#include <co/surface.h>

#define FMT CO_REAL_OUT

enum {
  X, Y, Z
};

int
main(int argc, const char **v)
{
  Surface *surface;
  He *he;
  real *x, *y, *z;
  real p[3];
  real lo[3] = { -3, -3, -3 };
  real hi[3] = { 3, 3, 3 };
  real size = 0.25;

  v++;
  vec_argv(&v, p);
  y_inif(stdin, &he, &x, &y, &z);
  surface_ini(lo, hi, size, &surface);
  surface_update(surface, he, x, y, z);

  printf("%d\n", surface_inside(surface, p[X], p[Y], p[Z]));

  y_fin(he, x, y, z);
  surface_fin(surface);

  return 0;
}


/* 
	Put
	make
	./point 0 100 0 </u/0.off
	./point 0 0 0 </u/0.off 

*/
