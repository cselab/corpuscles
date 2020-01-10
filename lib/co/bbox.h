#define T Bbox
typedef struct T T;
int bbox_ini(T **);
int bbox_fin(T *);
int bbox_update(T *, int, const real *, const real *, const real *);
int bbox_lo(T *, real **);
int bbox_hi(T *, real **);
int bbox_center(T *, real[3]);
int bbox_inside(T *, real, real, real);
real bbox_xhi(T *);
real bbox_zhi(T *);

#undef T
