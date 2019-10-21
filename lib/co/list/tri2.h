#define T Tri2List

typedef struct T T;
typedef struct He He;
int tri2list_ini(const real lo[2], const real hi[2], real size, T **);
int tri2list_fin(T *);
int tri2list_push(T *, He *, const real *, const real *);
int tri2list_tris(T *, real, real, int **);

#undef T
