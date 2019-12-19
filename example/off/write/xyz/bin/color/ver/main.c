#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/off.h>
#include <co/he.h>
#include <co/memory.h>

#include <co/y.h>

static Off *off;
static real *x, *y, *z, *c;
static real lo, hi;
static He *he;

#define FMT_IN CO_REAL_IN

static int scl(char **v, /**/ real *p) {
    if (*v == NULL) ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

int main(int argc, char **argv)
{
  lo = 0; hi = 0.5;
  scl(++argv, &lo);
  scl(++argv, &hi);

  MSG("lo=%f hi=%f", lo, hi);
  y_inif(stdin, &he, &x, &y, &z);
  int n = he_nv(he);
  CALLOC(n, &c);
  for (int i=0;i<n;i++){
    //c[i]=(real) i/ 3.0;
    c[i]=(real) i/ (real) n;
    //c[i]=z[i];
  }
  //c[50]=1;
  //c[120]=2;
  
  boff_lh_ver_fwrite(he, x, y, z, lo, hi, c, stdout);
  //boff_ver_fwrite(he, x, y, z, c, stdout);
  y_fin(he, x, y, z);
  return 0;
}
