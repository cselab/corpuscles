#define T HeReal

struct T { };

int he_real_ini(__UNUSED int n, __UNUSED T **pq) { return HE_OK; }
int he_real_fin(__UNUSED T *q) { return HE_OK; }

double* he_real_to(__UNUSED T* q, real *a) { return a; }
real*   he_real_from(__UNUSED T* q, double *a) {return a; }

