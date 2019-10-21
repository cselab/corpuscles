#define T HeSum
typedef struct T T;
int he_sum_ini(T **);
int he_sum_fin(T *);
int he_sum_add(T *, real);
real he_sum_get(T *);
real he_sum_compensation(T *, real);
real he_sum_array(int, const real[]);

#undef T
