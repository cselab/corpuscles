#define T Bbox2
typedef struct T T;
int bbox2_ini(T **);
int bbox2_fin(T *);
int bbox2_update(T *, int, const real *, const real *);
int bbox2_lo(T *, real **);
int bbox2_hi(T *, real **);
int bbox2_inside(T *, real, real);
real bbox2_xhi(T *);
real bbox2_yhi(T *);

#undef T
