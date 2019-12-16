#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/remesh.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "sqrt3";
static void
usg()
{
  fprintf(stderr, "%s OFF > OFF\n", me);
  exit(2);
}

int
main(int argc, char **argv)
{
  USED(argc);
  He *he;
  He *he0;
  real *x;
  real *y;
  real *z;

  while (*++argv != NULL && argv[0][0] == '-')
    switch (argv[0][1]) {
    case 'h':
      usg();
      break;
    default:
      ER("unknown option '%s'", argv[0]);
    }
  y_inif(stdin, &he, &x, &y, &z);
  remesh_sqrt3(he, &x, &y, &z, &he0);
  if (off_he_xyz_fwrite(he0, x, y, z, stdout) != CO_OK)
    ER("off_he_xyz_fwrite failed");
  he_fin(he0);
  y_fin(he, x, y, z);
}
