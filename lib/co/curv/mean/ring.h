#define T CurvMeanRing

typedef struct He He;
typedef struct T T;

int curv_mean_ring_ini(He *, T **);
int curv_mean_ring_fin(T *);
int curv_mean_ring_apply(T *, He *, const real *, const real *,
                         const real *, /**/ real *);

//int curv_mean_ring_area(T *, /**/ real **);

#undef T
