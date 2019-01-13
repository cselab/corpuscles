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

#define T HePly
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
    MALLOC(1, &q);
    *pq = q;
    return HE_OK;
}
