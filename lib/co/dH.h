#define T Dh

typedef struct He He;
typedef struct T T;

struct dHParam {
    void *p;
     real(*dh) (void *, real a, real H);
     real(*da) (void *, real a, real H);
};
typedef struct dHParam dHParam;

int dh_ini(He *, /**/ T **);
int dh_fin(T *);
int dh_force(T *, dHParam, He *, const real *, const real *, const real *,
             /**/ real *, real *, real *);
int dh_area_h(T *, He *, const real *, const real *, const real *);

int dh_area(T *, real **);
int dh_h(T *, real **);
int dh_norm(T *, real **, real **, real **);

#undef T
