#define T Force

typedef struct He He;
typedef struct T T;

int force_ini(const char *name, void **param, He*, /**/ T**);
int force_argv(const char **pargv[], He*, /**/ T**); /* TODO */
int force_fin(T*);
int force_force(T*, He*, const real *x, const real *y, const real *z,
                /**/ real *fx, real *fy, real *fz);
real force_energy(T*, He*, const real *x, const real *y, const real *z);

const char* force_list();
int force_narg(const char*);

int force_area_ini(void *param[], He*, /**/ T**);
int force_garea_ini(void *param[], He*, /**/ T**);
int force_volume_ini(void *param[], He*, /**/ T**);
int force_juelicher_xin_ini(void *param[], He*, /**/ T**);
int force_edg_sq_ini(void *param[], He*, /**/ T**);
int force_harmonic_ini(void *param[], He*, /**/ T**);
int force_area_voronoi_ini(void *param[], He*, /**/ T**);
int force_garea_voronoi_ini(void *param[], He*, /**/ T**);

#undef T
