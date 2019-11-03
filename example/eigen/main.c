#include <stdio.h>
#include <real.h>
#include <co/eigen.h>
#include <co/err.h>
#include <co/macro.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT CO_REAL_OUT

static const char *me = "example/eigen";

int
main(__UNUSED int argc, char **argv)
{
  real *x, *y, *z;
  real vec[3*3], val[3];
  He *he;
  Eigen *eigen;
  const char *arg;
  int Type;
  enum {VECTOR, VALUE};

  argv++;
  Type = VECTOR;
  while (*argv != NULL) {
    arg = argv++[0];
    if (arg[0] != '-')
      break;
    switch (arg[1]) {
    case 'v':
      Type = VALUE;
      break;
    case 'e':
      Type = VECTOR;
      break;
    default:
      ER("%s: unknown option '%s'", me, arg);
    }
  }

  y_inif(stdin, &he, &x, &y, &z);
  eigen_ini(he, &eigen);

  eigen_vector_surface(eigen, he, x, y, z, vec);
  eigen_values(eigen, val);

  switch(Type) {
  case VECTOR:
    vec_printf(&vec[0], FMT);
    vec_printf(&vec[3], FMT);
    vec_printf(&vec[6], FMT);
    break;
  case VALUE:
    vec_printf(val, FMT);
    break;
  }
  eigen_fin(eigen);
  y_fin(he, x, y, z);
  return 0;
}
