#include <stdio.h>

#include "he/err.h"
#include "he/memory.h"
#include "he/read.h"

#include "he/he.h"

#define T He

struct T {
};

int he_fin(T *q) {
    FREE(q);
    return HE_OK;
}

