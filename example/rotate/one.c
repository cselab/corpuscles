#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/equiangulate.h>
#include <co/y.h>

const char *me = "rotate/one";
static void
usg()
{
    fprintf(stderr, "%s: [tirangle number] < OFF\n", me);
    exit(2);
}

int
main(int __UNUSED argc, const char **argv)
{
    He *he;
    real *x, *y, *z;
    const char *arg;
    int t, nt, Tflag;

    Tflag = 0;
    while (*++argv != NULL && argv[0][0] == '-')
	switch (argv[0][1]) {
	case 'h':
	    usg();
	    break;
	case 't':
	    argv++;
	    if ((arg = *argv) == NULL) {
		fprintf(stderr, "%s: -t needs an argument\n", me);
		exit(2);
	    }
	    Tflag = 1;
	    t = atoi(*argv);
	    break;
	default:
	    fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
	    exit(2);
	}
    y_inif(stdin, &he, &x, &y, &z);
    nt = he_nt(he);
    if (Tflag) {
	fprintf(stdin, "%s: triangle (-t) is not set\n", me);
	exit(2);
    }
    
    y_fin(he, x, y, z);
}
