#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/multi.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

const char *me = "loop/middle";
static void
usg()
{
    fprintf(stderr, "%s OFF > struct.c\n", me);
    exit(2);
}

#define COORD(x)						\
    do {							\
	printf("static double " Prefix #x "[%d] = {\n", nv);	\
	for (i = 0; i < nv; i++)				\
	    printf(TAB FMT ",\n", x[i]);			\
	puts("};");						\
    } while (0);						\

#define INDEX(x)						\
    do {							\
	printf("static int " Prefix #x "[%d] = {\n", nt);	\
	for (i = 0; i < nt; i++)				\
	    printf(TAB "%d" ",\n", x[i]);			\
	puts("};");						\
    } while (0);						\


int
main(int argc, char **argv)
{
    He *he;
    real *x, *y, *z;
    int *t0, *t1, *t2;
    int nv, nt, i;

    USED(argc);
    USED(argv);
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    he_T(he, &t0, &t1, &t2);
    printf("static int " Prefix "nv = %d;\n", nv);
    printf("static int " Prefix "nt = %d;\n", nt);
    COORD(x);
    COORD(y);
    COORD(z);
    INDEX(t0);
    INDEX(t1);
    INDEX(t2);
    y_fin(he, x, y, z);
}
