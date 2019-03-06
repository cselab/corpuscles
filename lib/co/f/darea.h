#define T HeFDarea

typedef struct He He;
typedef struct T T;

/* E = K * sum_{tri} area */
int he_f_darea_ini(real K, He*, T**);
int he_f_darea_argv(char***, He*, T**);
int he_f_darea_fin(T*);
int he_f_darea_force(T*, He*, const real*, const real*, const real*, /**/ real*, real*, real*);
real he_f_darea_energy(T*, He*, const real*, const real*, const real*);

#undef T
