#define T Filter

typedef struct He He;
typedef struct T T;

int filter_ini(He *, /**/ T **);
int filter_fin(T *);

int filter_apply(T *, He *, const real *, const real *, const real *,
                 /*io */ real * a);

#undef T
