#define T Force

typedef struct He He;
typedef struct T T;

int force_ini(const char *name, const real *param, He*, /**/ T**);
int force_fin(T*);

int force_force(T*, He*, const real *x, const real *y, const real *z,
                /**/ real *fx, real *fy, real *fz);
real force_energy(T*, He*, const real *x, const real *y, const real *z);
int force_energy_ver(T*, /**/ real**);
const char* force_list();

int force_area_ini(const real *param, He*, /**/ T**);

#undef T
