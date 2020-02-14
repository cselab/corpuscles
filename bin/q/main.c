#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <real.h>
#include <co/area.h>
#include <co/argv.h>
#include <co/array.h>
#include <co/bending.h>
#include <co/err.h>
#include <co/he.h>
#include <co/invariants.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/util.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static const char *me = "co.q";
static int q_al(void);
static int q_area(void);
static int q_bending(void);
static int q_be(void);
static int q_bfs(void);
static int q_id(void);
static int q_x(void);
static int q_y(void);
static int q_z(void);
static int eputs(const char *);
static int adj(He *, int, int *, int *, int *);

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

static const char *Name[] = {
    "al",
    "area",
    "be",
    "bending",
    "bfs",
    "id",
    "x",
    "y",
    "z",
};

static int (*Func[])(void) = {
    q_al,
    q_area,
    q_be,
    q_bending,
    q_bfs,
    q_id,
    q_x,
    q_y,
    q_z,
};

static void
usg(void)
{
    int i;

    fprintf(stderr, "%s query [ARGS..] < IN.off > OUT.off\n", me);
    fputs("color off file\n", stderr);
    fputs("possible queries:\n", stderr);
    for (i = 0; i < sizeof(Name) / sizeof(Name[0]); i++)
        eputs(Name[i]);
    exit(2);
}

static char **argv;
static real *x, *y, *z, *area;
static int nv, nt;
static He *he;
static int Lim;                 /* are limits set? */
static real lo, hi;

static int
divide(int n, const real * a, const real * b, /**/ real * c)
{
    int i;

    for (i = 0; i < n; i++)
        c[i] = a[i] / b[i];
    return CO_OK;
}

static int
ver(const real * a)
{
    if (Lim)
        return boff_lh_ver_fwrite(he, x, y, z, lo, hi, a, stdout);
    else {
        fprintf(stderr, FMT " " FMT "\n",
                array_min(nv, a), array_max(nv, a));
        return boff_ver_fwrite(he, x, y, z, a, stdout);
    }
}

static int
tri(const real * a)
{
    if (Lim)
        return boff_lh_tri_fwrite(he, x, y, z, lo, hi, a, stdout);
    else {
        fprintf(stderr, FMT " " FMT "\n",
                array_min(nt, a), array_max(nt, a));
        return boff_tri_fwrite(he, x, y, z, a, stdout);
    }
}

static int
nxt(const char *b)
{
    return (argv[0] != NULL) && util_eq(argv[0], b);
};

int
main(__UNUSED int c, char **v)
{
    char q[1024];
    int n, i;

    argv = v;
    argv++;
    if (nxt("-h"))
        usg();

    if (nxt("-l")) {
        Lim = 1;
        argv++;
        argv_real(&argv, &lo);
        if (!nxt("-h"))
            ER("expecting -h, got '%s'", argv[0] ? argv[0] : "null");
        argv++;
        argv_real(&argv, &hi);
    }

    argv_str(&argv, q);
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    MALLOC(nv, &area);
    he_area_ver(he, x, y, z, area);

    n = sizeof(Name) / sizeof(Name[0]);
    for (i = 0; i < n; i++) {
        if (util_eq(q, Name[i])) {
            Func[i] ();
            goto ok;
        }
    }

    MSG("unknown query: '%s'", q);
    eputs("possible values are:");
    for (i = 0; i < n; i++)
        eputs(Name[i]);
    return 1;
  ok:
    FREE(area);
    y_fin(he, x, y, z);
    return 0;
}

static int
q_id(void)
{
    int i;
    real *a;

    MALLOC(nt, &a);
    for (i = 0; i < nt; i++)
        a[i] = i;
    tri(a);
    FREE(a);
    return CO_OK;
}

static int
q_x(void)
{
    return ver(x);
}

static int
q_y(void)
{
    return ver(y);
}

static int
q_z(void)
{
    return ver(z);
}

static int
q_area(void)
{
    real *a;

    MALLOC(nt, &a);
    he_area_tri(he, x, y, z, a);
    tri(a);
    FREE(a);
    return CO_OK;
}

static int
q_bending(void)
{
    Bending *b;
    BendingParam p;
    real *eng, *deng;

    MALLOC(nv, &deng);
    p.Kb = 1;
    p.C0 = p.Kad = p.DA0D = 0;

    bending_ini("juelicher_xin", p, he, &b);
    bending_energy(b, he, x, y, z);
    bending_energy_ver(b, &eng);
    he_area_ver(he, x, y, z, area);

    divide(nv, eng, area, deng);

    ver(deng);

    bending_fin(b);
    FREE(deng);
    return CO_OK;
}

static int
q_al(void)
{
    real *i;
    Invariants *invariants;
    char off[1024];

    argv_str(&argv, off);
    invariants_ini(off, &invariants);
    invariants_al(invariants, x, y, z, &i);
    tri(i);
    invariants_fin(invariants);
    return CO_OK;
}

static int
q_be(void)
{
    real *i;
    Invariants *invariants;
    char off[1024];

    argv_str(&argv, off);
    invariants_ini(off, &invariants);
    invariants_be(invariants, x, y, z, &i);
    tri(i);
    invariants_fin(invariants);
    return CO_OK;
}

static int
q_bfs(void)
{
#define VISIT(i)		   \
    if (distance[i] == -1) {	   \
	distance[i] = distance[t] + 1;		\
	queue_enqueue(i);			\
    }

    int *distance;
    real *a;
    int status;
    int t;
    int i;
    int j;
    int k;

    t = 0;
    for (; *argv != NULL && argv[0][0] == '-'; argv++)
        switch (argv[0][1]) {
        case 't':
            if (*++argv == NULL) {
                fprintf(stderr, "%s: -t needs an argument\n", me);
                exit(2);
            }
            argv_int(&argv, &t);
            break;
        default:
            fprintf(stderr, "%s: unknown bfs option '%s'\n", me, argv[0]);
            exit(2);
            break;
        }
    if (t < 0 && t >= nt) {
        fprintf(stderr, "%s: t=%d is not in [0, %d)\n", me, t, nt);
        exit(2);
    }
    MALLOC(nt, &distance);
    for (i = 0; i < nt; i++)
        distance[i] = -1;
    MALLOC(nt, &a);
    queue_ini(nt);
    queue_enqueue(t);
    distance[t] = 0;
    for (;;) {
        queue_empty(&status);
        if (status == 1)
            break;
        queue_dequeue(&t);
        adj(he, t, &i, &j, &k);
        VISIT(i);
        VISIT(j);
        VISIT(k);
    }
    for (i = 0; i < nt; i++) {
        assert(distance[i] != -1);
        a[i] = distance[i];
    }
    queue_fin();
    FREE(distance);
    tri(a);
    FREE(a);
    return CO_OK;
}

int
eputs(const char *s)
{
    fputs(s, stderr);
    fputs("\n", stderr);
    return CO_OK;
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

static int
adj(He * he, int t, int *pi, int *pj, int *pk)
{
    int i;
    int j;
    int k;

    i = he_hdg_tri(he, t);
    j = he_nxt(he, i);
    k = he_nxt(he, j);
    if (he_bnd(he, i) || he_bnd(he, j) || he_bnd(he, k)) {
        fprintf(stderr, "%s: does not work for open surface\n", me);
        exit(2);
    }
    i = he_flp(he, i);
    j = he_flp(he, j);
    k = he_flp(he, k);
    *pi = he_tri(he, i);
    *pj = he_tri(he, j);
    *pk = he_tri(he, k);
    return CO_OK;
}
