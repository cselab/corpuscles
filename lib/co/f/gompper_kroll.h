#define T HeFGompperKroll

typedef struct He He;
typedef struct T T;

int he_f_gompper_kroll_ini(real Kb, real C0, real Kad, real DA0D, He *,
                           T **);
int he_f_gompper_kroll_argv(char ***, He *, T **);
int he_f_gompper_kroll_fin(T *);
int he_f_gompper_kroll_force(T *, He *, const real *, const real *,
                             const real *, /**/ real *, real *, real *);
real he_f_gompper_kroll_energy(T *, He *, const real *, const real *,
                               const real *);
int he_f_gompper_kroll_area_ver(T *, /**/ real **);
int he_f_gompper_kroll_laplace_ver(T *, /**/ /**/ real **, real **,
                                   real **);
int he_f_gompper_kroll_norm_ver(T *, /**/ real **, real **, real **);
int he_f_gompper_kroll_curva_mean_ver(T *, /**/ real **);
int he_f_gompper_kroll_curva_gauss_ver(T *, /**/ real **);
int he_f_gompper_kroll_energy_ver(T *, /**/ real **);
real he_f_gompper_kroll_energy_ad(T *);
real he_f_gompper_kroll_energy_bend(T *);

#undef T
