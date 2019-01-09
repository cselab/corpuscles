#define T  HeFKantor

typedef struct He He;
typedef struct T T;

int he_f_kantor_ini(real Kb, real theta0, He*, T**);
int he_f_kantor_fin(T*);
int he_f_kantor_force(T*, He*, const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
real he_f_kantor_energy(T*, He*, const real *x, const real *y, const real *z);
int he_f_kantor_energy_ver(T*, /**/ real**);
int he_f_kantor_cos(T*, /**/ real**);

#undef T
