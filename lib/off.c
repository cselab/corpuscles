#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/util.h"
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
    *pq = q;
    return HE_OK;
}

int he_off_fin(T *q) {
    FREE(q->ver); FREE(q->tri);
    FREE(q);
    return HE_OK;
}
