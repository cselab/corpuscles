#define T CurvMeanJuelicher

typedef struct He He;
typedef struct T T;

int curv_mean_juelicher_ini(He *, T **);
int curv_mean_juelicher_fin(T*);
int curv_mean_juelicher_apply(T *, He *, const real *, const real *, const real *, /**/ real *);
int curv_mean_juelicher_area(T *, /**/ real **);

#undef T
