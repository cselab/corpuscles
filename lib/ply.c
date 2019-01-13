#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/util.h"
#include "he/he.h"
#include "inc/def.h"

#include "he/ply.h"

#define T Ply
enum {SIZE = MAX_STRING_SIZE};

#define FMT HE_REAL_IN

#   define FWRITE(ptr, size) \
        if (size != (cnt = fwrite(ptr, sizeof((ptr)[0]), size, f)))          \
            ERR(HE_IO, "fwrite failed: need = %d, got = %d", size, cnt)

struct Work {
    float *ver;
    int *tri;
};
typedef struct Work Work;

struct T {
    real *x, *y, *z;
    int  *tri; /* t0[0] t1[0] t2[0] ... */
    He *he;
    int nv, nt, nm;
    Work w;
};

static int get_nb(void) {
    int n;
    char *s;
    if ((s = getenv("nb")) == NULL)
        ERR(HE_IO, "environment variable `nb' is not set");
    if (sscanf(s, "%d", &n) != 1)
        ERR(HE_IO, "`nb = %s' is not integer", s);
    return n;
}

int ply_fread(FILE *f, T **pq) {
    T *q;
    int nb, nv, nt, nm, cnt, i, j, k;
    char line[SIZE];

#   define NXT() if (util_fgets(line, f) == NULL)  \
        ERR(HE_IO, "unexpected EOF")
#   define MATCH(s) do { \
        NXT();                                  \
        if (!util_eq(line, s)) {                \
            MSG("expecting: '%s'", s);          \
            ERR(HE_IO, "got: '%s'", line);      \
        }                                       \
    } while(0)
#   define FREAD(ptr, size) \
        if (size != (cnt = fread(ptr, sizeof((ptr)[0]), size, f)))          \
            ERR(HE_IO, "fread failed: need = %d, got = %d", size, cnt)
    MALLOC(1, &q);
    NXT();
    if (!util_eq(line, "ply"))
        ERR(HE_IO, "not a ply file");
    MATCH("format binary_little_endian 1.0");

    NXT();
    if (sscanf(line, "element vertex %d", &nv) != 1)
        ERR(HE_IO, "fail to parse: '%s'", line);
    if (nv < 0)
        ERR(HE_IO, "nv=%d < 0", nv);
    MATCH("property float x");
    MATCH("property float y");
    MATCH("property float z");
    MATCH("property float u");
    MATCH("property float v");
    MATCH("property float w");
    NXT();
    if (sscanf(line, "element face %d", &nt) != 1)
        ERR(HE_IO, "fail to parse: '%s'", line);
    if (nt < 0)
        ERR(HE_IO, "nt=%d < 0", nt);
    MATCH("property list int int vertex_index");
    MATCH("end_header");

    MALLOC(6*nv, &q->w.ver);
    MALLOC(4*nt, &q->w.tri);

    FREAD(q->w.ver, 6*nv);
    FREAD(q->w.tri, 4*nt);

    MALLOC(nv, &q->x);
    MALLOC(nv, &q->y);
    MALLOC(nv, &q->z);
    for (i = j = 0; i < nv; i++) {
        q->x[i] = q->w.ver[j++];
        q->y[i] = q->w.ver[j++];
        q->z[i] = q->w.ver[j++];
        j++; j++; j++; /* skip uvw */
    }

    nb = get_nb();
    if (nv % nb != 0)
        ERR(HE_IO, "nv=%d %% nb=%d != 0", nv, nb);

    nm = nv / nb;
    if (nt % nm != 0)
        ERR(HE_IO, "nt=%d %% nm=%d != 0", nv, nm);
    nt /= nm;
    nv /= nm;

    MALLOC(3*nt, &q->tri);
    for (i = j = k = 0; i < nt; i++) {
        j++;
        q->tri[k++] = q->w.tri[j++];
        q->tri[k++] = q->w.tri[j++];
        q->tri[k++] = q->w.tri[j++];
    }
    if (he_tri_ini(nv, nt, q->tri, &q->he) != HE_OK)
        ERR(HE_IO, "he_tri_ini failed");

    q->nv = nv;
    q->nt = nt;
    q->nm = nm;

    *pq = q;
    return HE_OK;
}

int ply_fin(T *q) {
    FREE(q->x); FREE(q->y); FREE(q->z);
    FREE(q->tri);
    FREE(q->w.ver); FREE(q->w.tri);
    FREE(q);
    return HE_OK;
}

int ply_nv(T *q) { return q->nv; }
int ply_nt(T *q) { return q->nt; }
int ply_nm(T *q) { return q->nm; }

int ply_he(T *q, He **p) {
    *p = q->he;
    return HE_OK;
}

int ply_tri(T *q, int **p) {
    *p = q->tri;
    return HE_OK;
}

int ply_x(T *q, int m, real **p) {
    if (m >= q->nm)
        ERR(HE_INDEX, "m=%d >= q->nm=%d", m, q->nm);
    *p = q->x + m*q->nv ;
    return HE_OK;
}

int ply_y(T *q, int m, real **p) {
    if (m >= q->nm)
        ERR(HE_INDEX, "m=%d >= q->nm=%d", m, q->nm);
    *p = q->y + m*q->nv ;
    return HE_OK;
}

int ply_z(T *q, int m, real **p) {
    if (m >= q->nm)
        ERR(HE_INDEX, "m=%d >= q->nm=%d", m, q->nm);
    *p = q->z + m*q->nv ;
    return HE_OK;
}

int ply_fwrite(T *q, FILE *f, int *b) {
    float *ver;
    int *wtri, *tri;
    int i, j, k, l, m, nv, nm, nt, cnt;
    int onm;
    real *x, *y, *z;

    ver = q->w.ver;
    wtri = q->w.tri;
    tri = q->tri;
    nv = q->nv; nm = q->nm; nt = q->nt;

    x = q->x; y = q->y; z = q->z;
    for (j = m = 0; m < nm; m++, x += nv, y += nv, z += nv) {
        if (b != NULL && b[m]) continue;
        for (i = 0; i < nv; i++) {
            ver[j++] = x[i];
            ver[j++] = y[i];
            ver[j++] = z[i];
            j++; j++; j++; /* skip uvw */
        }
    }

    for (j = m = k = 0; m < nm; m++) {
        if (b != NULL && b[m]) continue;
        for (i = l = 0; i < nt; i++) {
            wtri[j++] = 3;
            wtri[j++] = tri[l++] + k;
            wtri[j++] = tri[l++] + k;
            wtri[j++] = tri[l++] + k;
        }
        k += nv;
    }

    for (onm = m = 0; m < nm; m++) {
        if (b != NULL && b[m]) continue;
        onm++;
    }
    fprintf(f, "ply\n"
            "format binary_little_endian 1.0\n"
            "element vertex %d\n"
            "property float x\n"
            "property float y\n"
            "property float z\n"
            "property float u\n"
            "property float v\n"
            "property float w\n"
            "element face %d\n"
            "property list int int vertex_index\n"
            "end_header\n", nv*onm, nt*onm);
    FWRITE(ver, 6*nv*onm);
    FWRITE(wtri, 4*nt*onm);
    
    return HE_OK;
}
