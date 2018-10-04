#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/util.h"
#include "he/he.h"
#include "inc/def.h"

#include "he/off.h"

#define T HeOff
enum {SIZE = MAX_STRING_SIZE};
#define E(fmt, ...) ERR(HE_IO, fmt, ##__VA_ARGS__);
#define FMT XE_REAL_IN

struct T {
    real *ver; /* x[0] y[0] z[0] ... */
    int  *tri; /* t0[0] t1[0] t2[0] ... */
    int nv, nt;
};

int he_off_ini(const char *path, T **pq) {
    T *q;
    FILE *f;
    char line[SIZE];
    int i, nv, nt;
    int *t0, *t1, *t2, cnt, np;
    real *ver, *x, *y, *z;
    int *tri;

    MALLOC(1, &q);
    f = fopen(path, "r");
    if (f == NULL) E("fail to open '%s'", path);

#   define NXT() if (util_comment_fgets(line, f) == NULL)  \
        E("unexpected EOF in '%s'", path)
    NXT();
    if (!util_eq(line, "OFF"))
        E("'%s' is not an off file", path);
    NXT();
    cnt = sscanf(line, "%d %d %*d", &nv, &nt);
    if (3*nt < nv)
        E("3*(nt=%d)   <   nv=%d", nt, nv);

    MALLOC(3*nv, &q->ver); ver = q->ver;
    MALLOC(3*nt, &q->tri); tri = q->tri;

    if (cnt != 2)
        E("'%s' != [nv nt ne] in '%s'", line, path);
    for (i = 0; i < nv; i++) {
        NXT();
        x = ver++; y = ver++; z = ver++;
        cnt  = sscanf(line, FMT " " FMT " " FMT, x, y, z);
        if (cnt != 3)
            E("wrong ver line '%s' in '%s'", line, path);
    }

    for (i = 0; i < nt; i++) {
        NXT();
        t0 = tri++; t1 = tri++; t2 = tri++;
        cnt  = sscanf(line, "%d %d %d %d", &np, t0, t1, t2);
        if (cnt != 4)
            E("wrong tri line '%s' in '%s'", line, path);
        if (np != 3)
            E("not a triangle '%s' in '%s'", line, path);
    }
    q->nv = nv; q->nt = nt;
    *pq = q;
    return HE_OK;
}

int he_off_fin(T *q) {
    FREE(q->ver); FREE(q->tri);
    FREE(q);
    return HE_OK;
}

int he_off_nt(T *q) { return q->nt; };
int he_off_nv(T *q) { return q->nv; };
int he_off_ver(T *q, real **p) { *p = q->ver; return HE_OK; }
int he_off_tri(T *q, int  **p) { *p = q->tri; return HE_OK; }

int he_off_fwrite(T *q, const real *x, const real *y, const real *z, /**/ FILE *f) {
    int nv, nt, ne, npv, *tri, m, i, j, k;
    if (fputs("OFF\n", f) == EOF)
        ERR(HE_IO, "fail to write");
    nv = q->nv; nt = q->nt; ne = 0; npv = 3;
    tri = q->tri;

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (m = 0; m < nv; m++)
        fprintf(f, "%.16e %.16e %.16e\n", x[m], y[m], z[m]);

    for (m = 0; m < nt; m++) {
        i = *tri++; j = *tri++; k = *tri++;
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return HE_OK;
}

int he_off_write(T *q, const real *x, const real *y, const real *z, /**/ const char *path) {
    FILE *f;
    if ((f = fopen(path, "w")) == NULL)
        ERR(HE_IO, "fail to open '%s'", path);
    if (he_off_fwrite(q, x, y, z, f) != HE_OK)
        ERR(HE_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(HE_IO, "fail to close '%s'", path);
    return HE_OK;
}

int he_off_tri_fwrite(T *q, const int *tri, /**/ FILE *f) {
    int nv, nt, ne, npv, m, i, j, k;
    real x, y, z;
    const real *ver;

    ver = q->ver;
    if (fputs("OFF\n", f) == EOF)
        ERR(HE_IO, "fail to write");
    nv = q->nv; nt = q->nt; ne = 0; npv = 3;

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (i = m = 0; m < nv; m++) {
        x = ver[i++]; y = ver[i++]; z = ver[i++];
        fprintf(f, "%.16e %.16e %.16e\n", x, y, z);
    }

    for (m = 0; m < nt; m++) {
        i = *tri++; j = *tri++; k = *tri++;
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return HE_OK;
}

int he_off_tri_write(T *q, const int *tri, /**/ const char *path) {
    FILE *f;
    if ((f = fopen(path, "w")) == NULL)
        ERR(HE_IO, "fail to open '%s'", path);
    if (he_off_tri_fwrite(q, tri, f) != HE_OK)
        ERR(HE_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(HE_IO, "fail to close '%s'", path);
    return HE_OK;
}

int he_off_he_fwrite(T *q, He *he, /**/ FILE *f) {
    int nv, nt, ne, npv, m, h, n, nn, i, j, k;
    real x, y, z;
    const real *ver;

    ver = q->ver;
    if (fputs("OFF\n", f) == EOF)
        ERR(HE_IO, "fail to write");
    nv = he_nv(he); nt = he_nt(he); ne = 0; npv = 3;
    if (q->nv != nv)
        ERR(HE_INDEX, "q->nv=%d   !=   nv=%d", q->nv, nv);

    fprintf(f, "%d %d %d\n", nv, nt, ne);
    for (i = m = 0; m < nv; m++) {
        x = ver[i++]; y = ver[i++]; z = ver[i++];
        fprintf(f, "%.16e %.16e %.16e\n", x, y, z);
    }

    for (m = 0; m < nt; m++) {
        h = he_hdg_tri(he, m);
        n = he_nxt(he, h);
        nn = he_nxt(he, n);
        i = he_ver(he, h); j = he_ver(he, n); k = he_ver(he, nn);
        fprintf(f, "%d %d %d %d\n", npv, i, j, k);
    }
    return HE_OK;
}

int he_off_he_write(T *q, He *he, /**/ const char *path) {
    FILE *f;
    if ((f = fopen(path, "w")) == NULL)
        ERR(HE_IO, "fail to open '%s'", path);
    if (he_off_he_fwrite(q, he, f) != HE_OK)
        ERR(HE_IO, "fail to write to '%s", path);
    if (fclose(f) != 0)
        ERR(HE_IO, "fail to close '%s'", path);
    return HE_OK;
}
