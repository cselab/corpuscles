#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

struct T {
    real *x, *y, *z;
    int  *tri; /* t0[0] t1[0] t2[0] ... */
    He *he;
    int nv, nt, nm;
};

int ply_fread(FILE *f, T **pq) {
    T *q;
    int nv, nt, cnt;
    float *ver0;
    int *tri0;
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

    MALLOC(3*nv, &ver0);
    MALLOC(4*nt, &tri0);

    FREAD(ver0, 3*nv);
    FREAD(tri0, 4*nt);

    FREE(ver0);
    FREE(tri0);
    *pq = q;
    return HE_OK;
}

int ply_fin(T *q) {
    FREE(q);
    return HE_OK;
}
