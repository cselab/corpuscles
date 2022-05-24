#include <stdio.h>
#include <math.h>
#include <real.h>
#include <co/err.h>
#include <co/vtk.h>
#include <co/y.h>

static const char *me = "co.2vtk";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > VTK\n", me);
    exit(1);
}

int
main(int argc, char **argv)
{
    real *x, *y, *z;
    He *he;

    while (*++argv != NULL && argv[0][0] == '-')
      switch (argv[0][1]) {
      case 'h':
	usg();
	break;
      default:
	fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	exit(1);
      }

    y_inif(stdin, &he, &x, &y, &z);
    vtk_write(he, x, y, z, NULL, NULL, stdout);

    y_fin(he, x, y, z);
    return 0;
}
