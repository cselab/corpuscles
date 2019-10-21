#define T Edg1List

typedef struct T T;
typedef struct Skel Skel;
int edg1list_ini(const real lo, const real hi, real size, T **);
int edg1list_fin(T *);
int edg1list_push(T *, Skel *, const real *);
int edg1list_edgs(T *, real, int **);

#undef T
