#define T HeFGompper

typedef struct He He;
typedef struct T T;

int he_f_gompper_ini(real Kb, real C0, real Kad, real DA0D, He *, T **);
int he_f_gompper_argv(char ***, He *, T **);
int he_f_gompper_fin(T *);
int he_f_gompper_force(T *, He *, const real *, const real *, const real *,
                       /**/ real *, real *, real *);
real he_f_gompper_energy(T *, He *, const real *, const real *,
                         const real *);
int he_f_gompper_area_ver(T *, /**/ real **);
int he_f_gompper_laplace_ver(T *, /**/ /**/ real **, real **, real **);
int he_f_gompper_norm_ver(T *, /**/ real **, real **, real **);
int he_f_gompper_curva_mean_ver(T *, /**/ real **);
int he_f_gompper_curva_gauss_ver(T *, /**/ real **);
int he_f_gompper_energy_ver(T *, /**/ real **);
real he_f_gompper_energy_ad(T *);
real he_f_gompper_energy_bend(T *);

#undef T
