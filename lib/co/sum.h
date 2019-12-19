#define T HeSum
typedef struct T T;
int sum_ini(T **);
int sum_fin(T *);
int sum_add(T *, real);
real sum_get(T *);
real sum_compensation(T *, real);
real sum_array(int, const real[]);

#undef T
