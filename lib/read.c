#include <stdio.h>
#include <string.h>

#include "inc/def.h"
#include "he/err.h"
#include "he/memory.h"
#include "he/read.h"
#include "he/macro.h"
#include "he/util.h"

#define T HeRead
#define SIZE (MAX_STRING_SIZE)
#define E(fmt, ...) ERR(HE_IO, fmt, ##__VA_ARGS__);

struct T {
    int nv, nt, ne, nh;
    int *next, *flip;
    int *ver, *tri, *edg;

    int *hdg_ver, *hdg_edg, *hdg_tri;
};

int he_read_ini(const char *path, T **pq) {
    T *q;
    FILE *f;
    char line[SIZE];
    int i, cnt;
    int nt, nv, ne, nh;
    int *next, *flip, *ver, *tri, *edg;
    int *hdg_ver, *hdg_edg, *hdg_tri;

#   define NXT() if (util_fgets(line, f) == NULL)  \
        E("fail to read '%s'", path)
    MALLOC(1, &q);
    *pq = q;
    f = fopen(path, "r");
    if (f == NULL) E("fail to open '%s'", path);
    NXT();
    if (!util_eq(line, "HE"))
        E("'%s' is not a he file", path);
    NXT()
    cnt = sscanf(line, "%d %d %d %d", &nv, &nt, &ne, &nh);
    if (cnt != 4)
        E("'%s' != [nv nt ne nh] in '%s'", line, path);
    if (nv <= 0 || nt <= 0 || ne <= 0 || nh <= 0)
        E("wrong sizes '%s' in '%s'", line, path);

    MALLOC(nh, &next); MALLOC(nh, &flip);
    MALLOC(nh, &ver); MALLOC(nh, &tri); MALLOC(nh, &edg);
    MALLOC(nv, &hdg_ver);
    MALLOC(ne, &hdg_edg);
    MALLOC(nt, &hdg_tri);
    for (i = 0; i < nh; i++) {
        NXT();
        cnt = sscanf(line, "%d %d  %d %d %d",
                     &next[i], &flip[i], &ver[i], &tri[i], &edg[i]);
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
    return HE_OK;
}
