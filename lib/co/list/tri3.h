#define T Tri3List

typedef struct T T;
typedef struct He He;
int tri3list_ini(const real lo[3], const real hi[3], real size, T **);
int tri3list_fin(T *);
int tri3list_push(T *, He *, const real *, const real *, const real *);

int tri3list_get(T *, real, real, real);
int tri3list_status(T *);
int tri3list_tri(T *);
int tri3list_tris(T *, int **);
int tri3list_point(T *, /**/ real[3]);

#undef T
