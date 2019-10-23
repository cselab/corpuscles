#define T CurvMean

typedef struct He He;
typedef struct T T;

int curv_mean_ini(He *, T **);
int curv_mean_fin(T*);
int curv_mean_apply(T *, He *, const real *, const real *, const real *, /**/ real *);

#undef T
