#define T HeFStrain

typedef struct He He;
typedef struct T T;

typedef struct StrainParam StrainParam;

int he_f_strain_ini(const char *off, const char *, StrainParam, T **);
int he_f_strain_argv(char ***, He *, T **);
int he_f_strain_fin(T *);
int he_f_strain_force(T *, He *, const real *, const real *, const real *, /**/ real *, real *, real *);
real he_f_strain_energy(T *, He *, const real *, const real *, const real *);
int he_f_strain_energies(T *, real *, real *);
int he_f_strain_energy_ver(T *, /**/ real **);
int he_f_strain_energies_ver(T *, /**/ real **, real **);
int he_f_strain_invariants(T *, const real *, const real *, const real *, /**/ real **, real **);

#undef T
