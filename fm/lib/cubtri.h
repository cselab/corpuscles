#define T Cubtri
typedef struct T T;

int cubtri_ini(T **);
int cubtri_fin(T *);
int cubtri_apply(T *, const real[3], const real[3], const real[3],
		 real(*)(real, real, real, void *), void *,
		 /**/ real *);

#undef T
