#define T Force

typedef struct He He;
typedef struct T T;

int force_ini(const char *name, const real *param, He*, /**/ T**);
int force_argv(const char **pargv[], He*, /**/ T**); /* TODO */
int force_fin(T*);
int force_force(T*, He*, const real *x, const real *y, const real *z,
                /**/ real *fx, real *fy, real *fz);
real force_energy(T*, He*, const real *x, const real *y, const real *z);

const char* force_list();
int force_narg(const char*);


int force_area_ini(const real *param, He*, /**/ T**);
int force_garea_ini(const real *param, He*, /**/ T**);
int force_volume_ini(const real *param, He*, /**/ T**);
int force_juelicher_xin_ini(const real *param, He*, /**/ T**);

#undef T
