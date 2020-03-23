#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/y.h>

#define FMT  CO_REAL_OUT
#define FMT3 FMT " " FMT " " FMT "\n"

static const char *me = "viscosity/pairwise";
static void
usg(void)
{
    fprintf(stderr, "%s < off\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
  He *he;
  real *x;
  real *y;
  real *z;
  err_set_ignore();
  USED(argc);
  while (*++argv != NULL && argv[0][0] == '-')
    switch (argv[0][1]) {
    case 'h':
      usg();
      break;
    default:
      fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
      exit(2);
      break;
    }
  if (y_inif(stdin, &he, &x, &y, &z) != 0) {
    fprintf(stderr, "%s: y_inif failed\n", me);
    exit(2);
  }
  y_fin(he, x, y, z);
}
