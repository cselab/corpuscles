#define T HeFArea

typedef struct He He;
typedef struct T T;

/*
E = K/area0 * sum_{tri} (area - area0)^2 
or
E = K*area0 * sum_{tri} (area/area0 - 1)^2
*/
int he_f_area_ini(real a0, real K, He *, T **);
int he_f_area_argv(char ***, He *, T **);
int he_f_area_fin(T *);
int he_f_area_force(T *, He *, const real *, const real *, const real *,
                    /**/ real *, real *, real *);
real he_f_area_energy(T *, He *, const real *, const real *, const real *);
int he_f_area_a(T *, /**/ real **);
int he_f_area_da(T *, /**/ real **);

#undef T
