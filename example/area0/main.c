#include <stdio.h>
#include <real.h>
#include <co/area.h>
#include <co/y.h>

int main() {
  He *he;
  real *x, *y, *z;
  y_inif(stdin, &he, &x, &y, &z);
  printf(CO_REAL_OUT "\n", he_area(he, x, y, z));
  y_fin(he, x, y, z);
  return 0;
}
