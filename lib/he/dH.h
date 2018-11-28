#define T Dh

typedef struct He He;
typedef struct T T;

struct dHParam {
    void *p;
    real (*dh)(void*, real a, real H);
    real (*da)(void*, real a, real H);
};
typedef struct dHParam dHParam;

int dh_ini(He*, /**/ T**);
int dh_fin(T*);
int dh_apply(T*, dHParam, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);

int dh_area_h(T*, He*, const real *x, const real *y, const real *z);

int dh_area(T*, real**);
int dh_h(T*, real**);

#undef T
