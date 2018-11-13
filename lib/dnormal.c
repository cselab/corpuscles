#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/ten.h"

#include "he/dnormal.h"

#define T Dnormal

struct T {
    int n;
};

int dnormal_ini(He *he, /**/ T **pq) {
    T *q;
    MALLOC(1, &q);

    *pq = q;
    return HE_OK;
}


int dnormal_fin(T *q) {
    FREE(q);
    return HE_OK;
}

int dnormal_apply(T *q, /**/ Ten **pt) {

}
