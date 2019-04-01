#define T Cell3
typedef struct Cell3 Cell3;
int cell3_nn_ini(const real lo[3], const real hi[3], real size, T**);
int cell3_np_ini(const real lo[3], const real hi[3], real size, T**);
int cell3_pn_ini(const real lo[3], const real hi[3], real size, T**);
int cell3_pp_ini(const real lo[3], const real hi[3], real size, T**);
int cell3_push(T*, int, const real*, const real*, const real*);
int cell3_parts(T*, real, real, real, int**);
int cell3_len(T*, real, real, real);
int cell3_wrap(T*, int, /**/ real*, real*, real*);
int cell3_bring(T*, real, real, real, /**/ real*, real*, real*);
int cell3_fin(T*);

#undef T