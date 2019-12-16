#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static const char *me = "generation/main";
static void
usg()
{
    fprintf(stderr, "%s OFF OFF > OFF\n", me);
    exit(2);
}

#define T Generation
struct T {
    int *g;
    int N;                      /* alloc size of g */
};
typedef struct T T;
int
generation_ini(He * he, T ** pq)
{
    int i;
    int n;
    T *q;

    MALLOC(1, &q);
    n = he_nt(he);
    MALLOC(n, &q->g);
    q->N = n;
    for (i = 0; i < n; i++)
	q->g[i] = 0;
    *pq = q;
    return CO_OK;
}

int
generation_fin(T * q)
{
    FREE(q->g);
    return CO_OK;
}

static int
realloc0(T * q, int n)
{
    if (n > q->N)
        q->N *= 2;
    REALLOC(q->N, &q->g);
    return CO_OK;
}

int
main(int argc, char **argv)
{
    He *he;
    real *x;
    real *y;
    real *z;

    USED(argc);
    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            fprintf(stderr, "%s: unknown option '%s'\n", me, argv[0]);
            exit(2);
        }
    y_inif(stdin, &he, &x, &y, &z);
    y_fin(he, x, y, z);
}
