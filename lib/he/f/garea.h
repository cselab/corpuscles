#define T HeFGarea

typedef struct He He;
typedef struct T T;

/* E = K/area0 * sum_{tri} (area_{tri} - area0)^2 */
int he_f_garea_ini(real a0, real K, He*, T**);
int he_f_garea_fin(T*);

int he_f_garea_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_garea_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_garea_a(T*, /**/ real  **a);
int he_f_garea_da(T*, /**/ real **da);

#undef T
