#define T HeReal

typedef struct T T;

int he_real_ini(int n, T**);
int he_real_fin(T*);

double* he_real_to(T*, real*);
real*   real_from(T*, double*);

#undef T
