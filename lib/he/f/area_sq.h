#define T HeFAreaSq

typedef struct He He;
typedef struct T T;

/* E = K/area0 * sum_{tri} (area_sq_{tri} - area0)^2 */
int he_f_area_sq_ini(real a0, real K, He*, T**);
int he_f_area_sq_fin(T*);

int he_f_area_sq_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_area_sq_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_area_sq_a(T*, /**/ real  **a);
int he_f_area_sq_da(T*, /**/ real **da);

#undef T
