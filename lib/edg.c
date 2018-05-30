#include <stdio.h>
#include "he/memory.h"
#include "he/err.h"
#include "he/edg.h"

#define T HeEdg
typedef struct Node Node;
struct Node {
    int v;
    Node  *nxt;
};
struct T {
    int n;
    Node *node;
};

int he_edg_ini(int n, T **pq) {
    T *q;
    MALLOC(1, &q);

    *pq = q;
    return HE_OK;
}
