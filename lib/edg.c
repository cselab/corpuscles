#include <stdio.h>
#include "he/memory.h"
#include "he/err.h"

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

