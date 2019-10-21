#define T Edg2List

typedef struct T T;
typedef struct Skel Skel;
int edg2list_ini(const real lo[2], const real hi[2], real size, T **);
int edg2list_fin(T *);

int edg2list_push(T *, Skel *, const real *, const real *);
int edg2list_get(T *, real, real);
int edg2list_status(T *);
int edg2list_edg(T *);
int edg2list_edgs(T *, /**/ int **);
int edg2list_point(T *, /**/ real[2]);

#undef T
