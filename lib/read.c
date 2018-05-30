#include <stdio.h>
#include <string.h>

#include "inc/def.h"
#include "he/err.h"
#include "he/memory.h"
#include "he/macro.h"
#include "he/util.h"

#include "he/read.h"

#define T HeRead
#define SIZE (MAX_STRING_SIZE)
#define E(fmt, ...) ERR(HE_IO, fmt, ##__VA_ARGS__);
#define MAGIC (42)

struct T {
    int nv, nt, ne, nh;
    int *nxt, *flp;
    int *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;
    int magic;
};


enum {OK, FAIL};
static int valid(int n, int *a, int *b) {
    int i, i0;
    for (i = 0; i < n; i++) {
        i0 = b[a[i]];
        if (i != i0) {
            MSG("i=%d a[i]=%d i0=%d ", i, a[i], i0);
            return FAIL;
        }
    }
    return OK;
}

int he_read_ini(const char *path, T **pq) {
    T *q;
    FILE *f;
    char line[SIZE];
    int i, cnt;
    int nv, nt, ne, nh;
    int *nxt, *flp, *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;

#   define NXT() if (util_fgets(line, f) == NULL)  \
        E("unexpected EOF in '%s'", path)
    MALLOC(1, &q);
    f = fopen(path, "r");
    if (f == NULL) E("fail to open '%s'", path);
    NXT();
    if (!util_eq(line, "HE"))
        E("'%s' is not a he file", path);
    NXT();
    cnt = sscanf(line, "%d %d %d %d", &nv, &ne, &nt, &nh);
    if (cnt != 4)
        E("'%s' != [nv nt ne nh] in '%s'", line, path);
    if (nv <= 0 || nt <= 0 || ne <= 0 || nh <= 0)
        E("wrong sizes '%s' in '%s'", line, path);

    MALLOC(nh, &nxt); MALLOC(nh, &flp);
    MALLOC(nh, &ver); MALLOC(nh, &tri); MALLOC(nh, &edg);
    MALLOC(nv, &hdg_ver);
    MALLOC(ne, &hdg_edg);
    MALLOC(nt, &hdg_tri);

    for (i = 0; i < nh; i++) {
        NXT();
        cnt = sscanf(line, "%d %d  %d %d %d",
                     &nxt[i], &flp[i], &ver[i], &edg[i], &tri[i]);
        if (cnt != 5) E("wrong half-edg line '%s' in '%s'", line, path);
    }
    for (i = 0; i < nv; i++) {
        NXT();
        cnt = sscanf(line, "%d", &hdg_ver[i]);
        if (cnt != 1) E("wrong ver line '%s' in '%s'", line, path);
    }
    for (i = 0; i < ne; i++) {
        NXT();
        cnt = sscanf(line, "%d", &hdg_edg[i]);
        if (cnt != 1) E("wrong edg line '%s' in '%s'", line, path);
    }
    for (i = 0; i < nt; i++) {
        NXT();
        cnt = sscanf(line, "%d", &hdg_tri[i]);
        if (cnt != 1) E("wrong tri line '%s' in '%s'", line, path);
    }
    if (fclose(f) != 0)
        E("fail to close file '%s'", path);

    q->nv = nv; q->nt = nt; q->ne = ne; q->nh = nh;

    q->nxt = nxt; q->flp = flp;
    q->ver = ver; q->tri = tri; q->edg = edg;

    q->hdg_ver = hdg_ver;
    q->hdg_edg = hdg_edg;
    q->hdg_tri = hdg_tri;
    q->magic = MAGIC;

    if (valid(nv, hdg_ver, ver) != OK)
        E("invalid ver references");
    if (valid(ne, hdg_edg, edg) != OK)
        E("invalid edg references");
    if (valid(nt, hdg_tri, tri) != OK)
        E("invalid tri references");    
    
    *pq = q;
    return HE_OK;
}

int he_read_fin(T *q) {
    if (q->magic != MAGIC)
        ERR(HE_MEMORY, "wrong fin() call");
    FREE(q->nxt); FREE(q->flp);
    FREE(q->ver); FREE(q->tri); FREE(q->edg);
    FREE(q->hdg_ver);
    FREE(q->hdg_edg);
    FREE(q->hdg_tri);
    FREE(q);
    return HE_OK;
}

int he_info(T *q, FILE *f) {
    int r;
    int nv, nt, ne, nh;
    int *nxt, *flp, *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;

    nv = q->nv; nt = q->nt; ne = q->ne; nh = q->nh;
    nxt = q->nxt; flp = q->flp;
    ver = q->ver; tri = q->tri; edg = q->edg;
    hdg_ver = q->hdg_ver;
    hdg_edg = q->hdg_edg;
    hdg_tri = q->hdg_tri;

    r = fprintf(f, "%d %d %d %d\n", nv, ne, nt, nh);
    if (r <= 0)
        ERR(HE_IO, "fprintf() failed");

    fprintf(f, "[nh=%d lines]\n", nh);
    fprintf(f, "%d %d %d %d %d\n",
            nxt[0], flp[0], ver[0], tri[0], edg[0]);
    fputs("...\n", f);
    fprintf(f, "%d %d %d %d %d\n",
            nxt[nh-1], flp[nh-1], ver[nh-1], tri[nh-1], edg[nh-1]);
    fprintf(f, "[nv=%d lines]\n", nv);
    fprintf(f, "%d\n", hdg_ver[0]);
    fputs("...\n", f);
    fprintf(f, "%d\n", hdg_ver[nv-1]);

    fprintf(f, "[ne=%d lines]\n", ne);
    fprintf(f, "%d\n", hdg_edg[0]);
    fputs("...\n", f);
    fprintf(f, "%d\n", hdg_edg[ne-1]);

    fprintf(f, "[nt=%d lines]\n", nt);
    fprintf(f, "%d\n", hdg_tri[0]);
    fputs("...\n", f);
    fprintf(f, "%d\n", hdg_tri[nt-1]);

    return HE_OK;
}

int he_read_nv(T *q) { return q->nv; }
int he_read_nt(T *q) { return q->nt; }
int he_read_ne(T *q) { return q->ne; }
int he_read_nh(T *q) { return q->nh; }

int he_read_nxt(T *q, int **p) { *p = q->nxt; return HE_OK; };
int he_read_flp(T *q, int **p) { *p = q->flp; return HE_OK; };
int he_read_ver(T *q,  int **p) { *p = q->ver; return HE_OK; };
int he_read_tri(T *q,  int **p) { *p = q->tri; return HE_OK; };
int he_read_edg(T *q,  int **p) { *p = q->edg; return HE_OK; };

int he_read_hdg_ver(T *q, int **p) { *p = q->hdg_ver; return HE_OK; };
int he_read_hdg_edg(T *q, int **p) { *p = q->hdg_edg; return HE_OK; };
int he_read_hdg_tri(T *q, int **p) { *p = q->hdg_tri; return HE_OK; };
