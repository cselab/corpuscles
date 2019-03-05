#define T Real

struct T { };

int real_ini(__UNUSED int n, __UNUSED T **pq) { return CO_OK; }
int real_fin(__UNUSED T *q) { return CO_OK; }

double* real_to(__UNUSED T* q, real *a) { return a; }
real*   real_from(__UNUSED T* q, double *a) {return a; }

