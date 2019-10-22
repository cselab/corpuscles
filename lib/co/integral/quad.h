#define T IntegralQuad

typedef struct T T;
int integral_quad_ini(T **);
int integral_quad_fin(T *);
int integral_quad_apply(T *, const real[3], const real[3], const real[3],
			real(*)(real, real, real, void *), void *,
			/**/ real *);

#undef T
