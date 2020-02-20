#include <stdio.h>
#include <stdlib.h>

#include <real.h>
#include <co/err.h>
#include <co/obj.h>

#include <co/y.h>

static const char *me = "co.obj";
static void
usg(void)
{
  fprintf(stderr, "%s < OFF > OBJ\n", me);
  exit(2);
}

int
main(int argc, char **argv)
{
    He *he;
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
    obj_fwrite(he, x, y, z, stdout);
    y_fin(he, x, y, z);
    return 0;
}
