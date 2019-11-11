#include <stdio.h>
#include <real.h>

#include <co/memory.h>
#include <co/vec.h>
#include <co/y.h>
#include <co/surface.h>

int
main(int argc, const char **argv)
{
  Surface *surface;
  He *he;
  real *x, *y, *z;
  real clo[3] = { -2, -2, -2 };
  real chi[3] = { 2, 2, 2 };
  real size = 0.2;

  y_inif(stdin, &he, &x, &y, &z);
  surface_ini(clo, chi, size, &surface);
  surface_fin(surface);
  y_fin(he, x, y, z);
}
