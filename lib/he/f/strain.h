#define T HeFStrain

typedef struct He He;
typedef struct T T;

typedef struct StrainParam StrainParam;

int he_f_strain_ini(const char *off, const  char *name, StrainParam, T**);
int he_f_strain_fin(T*);
int he_f_strain_force(T*, const real*, const real*, const real*, /**/ real*, real*, real*);
real he_f_strain_energy(T*, const real*, const real*, const real*);
int he_f_strain_energy_ver(T *q, /**/ real**);

#undef T
