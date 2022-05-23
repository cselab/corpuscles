#include <stdio.h>
#include <math.h>

#include <real.h>
#include <co/he.h>
#include <co/y.h>
#include <co/volume.h>

int main(void) {
  real *x, *y, *z, V;
  double a;
  double Vtetra;
  He *he;
  y_inif(stdin, &he, &x, &y, &z);
  a = sqrt(2*2 + 2*2);
  Vtetra = a*a*a/(6 * sqrt(2));
  printf(CO_REAL_OUT "\n"
	 "%.16g\n",
	 volume_tri(he, x, y, z), Vtetra);
  y_fin(he, x, y, z);
}
