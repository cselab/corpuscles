#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vtk.h>
#include <co/y.h>

static const char *me = "vtk/int";
static void
usg(void)
{
    fprintf(stderr, "%s < OFF > VTK\n", me);
    exit(2);
}

int
main(int argc, char **argv)
{
    USED(argc);
    real *x;
    real *y;
    real *z;
    He *he;
    int *id;
    int *neg;
    int n;
    int i;

    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	}
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nt(he);

    MALLOC(n, &id);
    MALLOC(n, &neg);
    for (i = 0; i < n; i++) {
	id[i] = i;
	neg[i] = -i;
    }
    
    const int *scalars[] = {id, neg, NULL};
    const char *names[] = {"id", "neg", NULL};
    vtk_tri_int_write(he, x, y, z, scalars, names, stdout);
    FREE(id);
    FREE(neg);
    y_fin(he, x, y, z);
}
