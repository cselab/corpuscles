#define T Real

struct T {
    int n;
    double *d;
    real *r;
};

int real_ini(int n, T **pq) {
    T *q;
    MALLOC(1, &q);
    q->n = n;
    MALLOC(n, &q->d);
    MALLOC(n, &q->r);
    *pq = q;
    return CO_OK;
}
int real_fin(T *q) {
    FREE(q->r); FREE(q->d);
    FREE(q);
    return CO_OK;
}

double* real_to(T *q, real *a) {
    int n, i;
    double *d;
    n = q->n;
    d = q->d;
    for (i = 0; i < n; i++) d[i] = a[i];
    return d;
}

real*   real_from(T *q, double *a) {
    int n, i;
    real *r;
    n = q->n;
    r = q->r;
    for (i = 0; i < n; i++) r[i] = a[i];
    return r;
}
