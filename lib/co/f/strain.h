#define T HeFStrain

typedef struct He He;
typedef struct T T;

typedef struct StrainParam StrainParam;

int he_f_strain_ini(const char *off, const char *, StrainParam, T **);
int he_f_strain_argv(char ***, He *, T **);
int he_f_strain_fin(T *);
int he_f_strain_force(T *, /*unused */ He *, const real *, const real *,
                      const real *, /**/ real *, real *, real *);
real he_f_strain_energy(T *, /*unused */ He *, const real *, const real *,
                        const real *);
int he_f_strain_energy_ver(T *, /**/ real **);
int he_f_strain_energy_tri(T *, /**/ real **);
int he_f_strain_invariants(T *, const real *, const real *, const real *,
                           /**/ real **, real **);
int he_f_strain_invariants_tri(T *, const real *, const real *,
                               const real *, /**/ real **, real **);

#undef T
