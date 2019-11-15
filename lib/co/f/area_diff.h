#define T HeFAreaDiff

typedef struct He He;
typedef struct T T;

/* E = K * sum_{tri}  (nt*area - Area)^2  
   Area = sum_{tri} area
*/
int he_f_area_diff_ini(real K, He *, T **);
int he_f_area_diff_argv(char ***, He *, T **);
int he_f_area_diff_fin(T *);
int he_f_area_diff_force(T *, He *, const real *, const real *,
                         const real *, /**/ real *, real *, real *);
real he_f_area_diff_energy(T *, He *, const real *, const real *,
                           const real *);

#undef T
