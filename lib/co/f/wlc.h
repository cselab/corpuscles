#define T HeFWlc

typedef struct He He;
typedef struct T T;

int he_f_wlc_ini(real x0, real K, const real *, const real *, const real *,
                 He *, T **);
int he_f_wlc_argv(char ***, He *, T **);
int he_f_wlc_fin(T *);
int he_f_wlc_force(T *, He *, const real *, const real *, const real *,
                   /**/ real *, real *, real *);
real he_f_wlc_energy(T *, He *, const real *, const real *, const real *);
int he_f_wlc_e(T *, /**/ real ** v);

#undef T
