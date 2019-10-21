#define T Cell2
typedef struct T T;
int cell2_nn_ini(const real lo[2], const real hi[2], real size, T **);
int cell2_np_ini(const real lo[2], const real hi[2], real size, T **);
int cell2_pn_ini(const real lo[2], const real hi[2], real size, T **);
int cell2_pp_ini(const real lo[2], const real hi[2], real size, T **);
int cell2_push(T *, int, const real *, const real *);
int cell2_parts(T *, real, real, int **);
int cell2_len(T *, real, real);
int cell2_wrap(T *, int, /**/ real *, real *);
int cell2_bring(T *, real, real, /**/ real *, real *);
int cell2_fin(T *);

#undef T
