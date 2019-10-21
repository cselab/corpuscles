#define T Da
typedef struct He He;
typedef struct T T;
struct dAParam {
    void *p;
     real(*da) (void *, real a);
};
typedef struct dAParam dAParam;
int da_ini(He *, /**/ T **);
int da_fin(T *);
int da_force(T *, dAParam, He *, const real *, const real *, const real *,
             /**/ real *, real *, real *);
int da_compute_area(T *, He *, const real *, const real *, const real *);
int da_area(T *, real **);

#undef T
