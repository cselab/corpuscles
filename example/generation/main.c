#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/he/invariant.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

static const char *me = "generation/main";
static void
usg()
{
    fprintf(stderr, "%s < OFF > OFF\n", me);
    exit(2);
}

static int tri_xyz(int t, He * he, const real * x, const real * y,
                   const real * z, real * u, real * v, real * w);

#define T Generation
struct T {
    int *g;
    int *mate;                  /* tri to hdg */
    int NT;                     /* alloc sizes */
    int NV;
    int *mbit;                  /* mate bit-flag */
};
typedef struct T T;
static int tri_mate(T *, int, He *);

static int bit_set(int generation, int *mbit);
static int bit_clear(int generation, int *mbit);
static int bit_get(int mbit, int generation, int *ans);
static int write_tri(int n, int *a, const char *name, FILE * file);

int
generation_ini(He * he, T ** pq)
{
    int i;
    int n;
    T *q;

    MALLOC(1, &q);
    n = he_nt(he);
    MALLOC(n, &q->g);
    MALLOC(n, &q->mate);
    MALLOC(n, &q->mbit);
    q->NT = n;
    q->NV = he_nv(he);
    for (i = 0; i < n; i++) {
        q->g[i] = 0;
        q->mbit[i] = 0;
    }
    *pq = q;
    return CO_OK;
}

int
generation_fin(T * q)
{
    FREE(q->g);
    FREE(q->mate);
    FREE(q->mbit);
    FREE(q);
    return CO_OK;
}

static int
realloc0(T * q, int nv, int nt, real ** x, real ** y, real ** z)
{
    if (nt > q->NT) {
        q->NT *= 2;
        REALLOC(q->NT, &q->g);
        REALLOC(q->NT, &q->mate);
        REALLOC(q->NT, &q->mbit);
    }
    if (nv > q->NV) {
        q->NV *= 2;
        REALLOC(q->NV, x);
        REALLOC(q->NV, y);
        REALLOC(q->NV, z);
    }
    return CO_OK;
}

static int
center(int t, He * he, const real * x, const real * y, const real * z,
       real d[3])
{
    int i;
    int j;
    int k;
    real a[3];
    real b[3];
    real c[3];

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    return tri_center(a, b, c, d);
}

static int
swap0(T * q, int t, He * he, int *status)
{
    int e;
    int m;                      /* triangle */

    if (q->g[t] % 2 == 0)
        ERR(CO_INDEX, "cannot swap even triangle (t=%d, g[t]=%d)", t,
            q->g[t]);
    e = he_edg(he, q->mate[t]);
    m = tri_mate(q, t, he);
    if (q->g[t] != q->g[m]) {
        *status = 1;
        return CO_OK;
    }
    if (he_edg_rotate(he, e) != CO_OK)
        ERR(CO_INDEX, "he_edg_rotate failed (e=%d)", e);
    q->g[t] += 1;
    q->g[m] += 1;
    *status = 0;
    return CO_OK;
}

static int
split0(T * q, int t, He * he, real ** x, real ** y, real ** z, int *pu,
       int *pv, int *pw)
{
    int a;
    int b;
    int Bit;
    int c;
    int Generation;
    int Mate;
    int nt;
    int nv;
    int u;
    int v;
    int w;
    real Center[3];

    if (q->g[t] % 2 != 0)
        ERR(CO_INDEX, "cannot split odd triangle (t=%d)", t);
    a = he_hdg_tri(he, t);      /* half edges */
    b = he_nxt(he, a);
    c = he_nxt(he, b);
    Generation = q->g[t];
    Bit = q->mbit[t];
    Mate = q->mate[t];
    center(t, he, *x, *y, *z, Center);
    if (he_tri_split3(he, t) != CO_OK)
        ERR(CO_INDEX, "he_tri_split3 failed (t=%d)", t);
    nv = he_nv(he);
    nt = he_nt(he);
    realloc0(q, nv, nt, x, y, z);
    vec_set(Center, nv - 1, *x, *y, *z);
    u = he_tri(he, a);          /* new triangles */
    v = he_tri(he, b);
    w = he_tri(he, c);
    q->mate[u] = a;
    q->mate[v] = b;
    q->mate[w] = c;
    q->g[u] = Generation + 1;
    q->g[v] = Generation + 1;
    q->g[w] = Generation + 1;
    q->mbit[u] = Bit;
    q->mbit[v] = Bit;
    q->mbit[w] = Bit;
    if (Generation > 0) {
        if (a == Mate)
            bit_set(Generation, &q->mbit[u]);
        if (b == Mate)
            bit_set(Generation, &q->mbit[v]);
        if (c == Mate)
            bit_set(Generation, &q->mbit[w]);
    }
    *pu = u;
    *pv = v;
    *pw = w;
    return CO_OK;
}

static int
split(T * q, int t, He * he, real ** x, real ** y, real ** z)
{
    int a;
    int b;
    int c;
    int status;

    if (split0(q, t, he, x, y, z, &a, &b, &c) != CO_OK)
        ERR(CO_INDEX, "split0 failed (t = %d)", t);
    swap0(q, a, he, &status);
    swap0(q, b, he, &status);
    swap0(q, c, he, &status);
    return CO_OK;
}

static int
tri_mate(T * q, int t, He * he)
{
    int m;

    m = q->mate[t];
    if (he_bnd(he, m))
        ERR(CO_INDEX, "cannot swap on the boundary (t=%d, m=%d)", t, m);
    m = he_flp(he, m);
    m = he_tri(he, m);
    return m;
}

int
generation_refine(T * q, int t, He * he, real ** x, real ** y, real ** z)
{
    int m;
    int status;

    if (q->g[t] % 2 == 0) {
        return split(q, t, he, x, y, z);
    } else {
        m = tri_mate(q, t, he);
        if (q->g[m] == q->g[t] - 2) {
            generation_refine(q, m, he, x, y, z);
            m = tri_mate(q, t, he);
        }
        return generation_refine(q, m, he, x, y, z);
    }
}

int
generation_invariant(T * q, He * he)
{
    int ne;
    int h;
    int f;
    int i;
    int j;
    int e;

    ne = he_ne(he);
    for (e = 0; e < ne; e++) {
        h = he_hdg_edg(he, e);
        f = he_flp(he, h);
        i = he_tri(he, h);
        j = he_tri(he, f);
        if (q->g[i] + 2 < q->g[j])
            ERR(CO_INDEX, "ij: %d %d", i, j);
        if (q->g[j] + 2 < q->g[i])
            ERR(CO_INDEX, "ij: %d %d", i, j);
    }
    return CO_OK;
}

static int
generation_write(T * q, He * he, const real * x, const real * y,
                 const real * z, FILE * file)
{
    int status;
    int h;
    int n;
    int t;
    int v;
    int *mver;

    n = he_nt(he);
    MALLOC(n, &mver);
    for (t = 0; t < n; t++)
        mver[t] = -1;
    for (t = 0; t < n; t++)
        if (q->g[t] % 2 == 1) {
            h = q->mate[t];
            h = he_nxt(he, h);
            h = he_nxt(he, h);
            v = he_ver(he, h);
        }
    fprintf(file, "{  ");
    if (fprintf(file, "LIST\n") < 0)
        ERR(CO_IO, "generation_write failed");
    fprintf(file, "{  ");
    if (off_he_xyz_fwrite(he, x, y, z, file) != CO_OK)
        ERR(CO_IO, "off_he_xyz_fwrite failed");
    fprintf(file, "}\n");
    write_tri(n, q->g, "g", file);
    write_tri(n, q->mbit, "mbit", file);
    write_tri(n, mver, "mver", file);   /* "mate ver" */
    fprintf(file, "}\n");
    FREE(mver);
    return CO_OK;
}


int
main(int argc, char **argv)
{
    He *he;
    real *color;
    real *x;
    real *y;
    real *z;
    int t;
    int nt;
    int i;
    real u;
    real v;
    real w;
    Generation *generation;

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
    generation_ini(he, &generation);
    nt = he_nt(he);
    for (i = 0; i < nt; i++) {
        tri_xyz(i, he, x, y, z, &u, &v, &w);
        if (u > 0)
            generation_refine(generation, i, he, &x, &y, &z);
    }
    nt = he_nt(he);
    for (i = 0; i < nt; i++) {
        tri_xyz(i, he, x, y, z, &u, &v, &w);
        if (v > 0)
            generation_refine(generation, i, he, &x, &y, &z);
    }
    generation_invariant(generation, he);

    nt = he_nt(he);
    MALLOC(nt, &color);
    for (i = 0; i < nt; i++)
        color[i] = generation->g[i];
    /* if (boff_tri_fwrite(he, x, y, z, color, stdout) != CO_OK)
       ER("boff_tri_fwrite failed"); */
    generation_write(generation, he, x, y, z, stdout);
    FREE(color);
    generation_fin(generation);
    y_fin(he, x, y, z);
}

static int
tri_xyz(int t, He * he, const real * x, const real * y, const real * z,
        real * u, real * v, real * w)
{
    enum { X, Y, Z };
    int i;
    int j;
    int k;
    real a[3];
    real b[3];
    real c[3];
    real d[3];

    he_tri_ijk(he, t, &i, &j, &k);
    vec_get3(i, j, k, x, y, z, a, b, c);
    tri_center(a, b, c, d);
    *u = d[X];
    *v = d[Y];
    *w = d[Z];
}


static int
bit_set(int n, int *mbit)
{
    if (n % 2 == 1)
        ERR(CO_INDEX, "generation cannot be odd");
    if (n > 50)
        ERR(CO_INDEX, "generation is too big (n = %d)", n);
    n /= 2;
    (*mbit) |= 1 << n;
    return CO_OK;
}

static int
bit_clear(int n, int *mbit)
{
    if (n % 2 == 1)
        ERR(CO_INDEX, "generation cannot be odd");
    if (n > 50)
        ERR(CO_INDEX, "generation is too big (n = %d)", n);
    n /= 2;
    (*mbit) &= ~(1 << n);
    return CO_OK;
}

static int
bit_get(int mbit, int n, int *ans)
{
    if (n % 2 == 1)
        ERR(CO_INDEX, "generation cannot be odd");
    if (n > 50)
        ERR(CO_INDEX, "generation is too big (n = %d)", n);
    n /= 2;
    *ans = (mbit >> n) & 1;
}

static int
write_tri(int n, int *a, const char *name, FILE * file)
{
    int i;

    if (fprintf(file, "{  ") < 0)
        ERR(CO_INDEX, "write_tri faield");
    fprintf(file, "COMMENT TRI_INT %s\n", name);
    fprintf(file, "{\n");
    for (i = 0; i < n; i++)
        fprintf(file, "%d\n", a[i]);
    fprintf(file, "}\n");
    fprintf(file, "}\n");
    return CO_OK;
}
