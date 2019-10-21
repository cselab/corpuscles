#define T  HeFKantor

typedef struct He He;
typedef struct T T;

int he_f_kantor_ini(real Kb, real theta0, He *, T **);
int he_f_kantor_argv(char ***, He *, T **);
int he_f_kantor_fin(T *);
int he_f_kantor_force(T *, He *, const real *, const real *, const real *,
                      /**/ real *, real *, real *);
real he_f_kantor_energy(T *, He *, const real *, const real *,
                        const real *);
int he_f_kantor_energy_ver(T *, /**/ real **);
int he_f_kantor_cos(T *, /**/ real **);
real he_f_kantor_energy_ad(T *);
real he_f_kantor_energy_bend(T *);

#undef T
