#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/remesh.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/edg.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "unloop";
static void
usg()
{
    fprintf(stderr, "%s OFF > OFF\n", me);
    exit(2);
}

struct {
    int n;
    int i;
    int j;
    int *q;
} Queue;
static int queue_ini(int);
static int queue_fin();
static int queue_enqueue(int);
static int queue_dequeue(int *);
static int queue_empty(int *);

int
main(int argc, char **argv)
{
    USED(argc);
    He *he;
    real *x;
    real *y;
    real *z;
    int n;
    int n5;
    int n6;
    int r;
    int start;
    int i;

    while (*++argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        default:
            ER("unknown option '%s'", argv[0]);
        }
    y_inif(stdin, &he, &x, &y, &z);
    n = he_nv(he);

    n6 = n5 = 0;
    for (i = 0; i < n; i++) {
        he_rank(he, i, &r);
        switch (r) {
        case 6:
            if (n6 == 0)
                start = i;
            n6++;
            break;
        case 5:
            n5++;
            break;
        default:
            fprintf(stderr, "%s: rank(%d) = %d. Should be 5 or 6.\n", me,
                    i, r);
            exit(2);
            break;
        }
    }
    if (n5 != 12) {
        fprintf(stderr, "%s: n5=%d != 12\n", me, n5);
        exit(2);
    }
    MSG("start: %d", start);

    int h;
    int a;
    int b;
    h = he_hdg_ver(he, start);
    a = he_tri(he, h);
    b = he_tri(he, he_nxt(he, h));
    
    y_fin(he, x, y, z);
}

static int
queue_ini(int n)
{
    MALLOC(n, &Queue.q);
    Queue.n = n;
    Queue.i = 0;
    Queue.j = 0;
    return CO_OK;
}

static int
queue_fin()
{
    FREE(Queue.q);
    return CO_OK;
}

static int
queue_enqueue(int x)
{
    assert(Queue.j <= Queue.n);
    Queue.q[Queue.j++] = x;
    return CO_OK;
}

static int
queue_dequeue(int *x)
{
    assert(Queue.i <= Queue.n);
    assert(Queue.i <= Queue.j);
    *x = Queue.q[Queue.i++];
    return CO_OK;
}

static int
queue_empty(int *status)
{
    *status = Queue.i == Queue.j;
    return CO_OK;
}
