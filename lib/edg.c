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
    Node **node;
};

int he_edg_ini(int n, T **pq) {
    T *q;
    int i;
    MALLOC(1, &q);
    MALLOC(n, &q->node);
    for (i = 0; i < n; i++)
        q->node[i] = NULL;
    *pq = q;
    return HE_OK;
}


static void free_node(Node *p) {
    Node *n;
    while (p != NULL) {
        n = p->nxt;
        FREE(p);
        p = n;
    }
}

int he_edg_fin(T *q) {
    int n, i;
    n = q->n;
    for (i = 0; i < n; i++)
        free_node(q->node[i]);
    FREE(q->node);
    FREE(q);
    return HE_OK;
}
