#define T Cell3
typedef struct T T;
int cell3_ppp_ini(const real lo[3], const real hi[3], real size, T **);
int cell3_nnn_ini(const real lo[3], const real hi[3], real size, T **);
int cell3_ppn_ini(const real lo[3], const real hi[3], real size, T **);
int cell3_pnn_ini(const real lo[3], const real hi[3], real size, T **);
int cell3_push(T *, int, const real *, const real *, const real *);
int cell3_parts(T *, real, real, real, int **);
int cell3_len(T *, real, real, real);
int cell3_wrap(T *, int, /**/ real *, real *, real *);
int cell3_bring(T *, real, real, real, /**/ real *, real *, real *);
int cell3_fin(T *);

#undef T
