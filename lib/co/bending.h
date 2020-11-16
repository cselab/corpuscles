#define T Bending
#define P BendingParam
typedef struct He He;
typedef struct T T;
typedef struct P P;
struct P {
    real Kb, C0, Kad, DA0D;
};
int bending_ini(const char *name, P, He *, /**/ T **);
int bending_fin(T *);
int bending_force(T *, He *, const real *, const real *, const real *,
                  /**/ real *, real *, real *);
real bending_energy(T *, He *, const real *, const real *, const real *);
int bending_energy_ver(T *, /**/ real **);
int bending_area_ver(T *, /**/ real **);
int bending_curva_mean_ver(T *, /**/ real **);
int bending_laplace_ver(T *, /**/ real **, real **, real **);
real bending_energy_bend(T *);
real bending_energy_ad(T *);
const char *bending_list();
int bending_kantor_ini(P, He *, /**/ T **);
int bending_gompper_ini(P, He *, /**/ T **);
int bending_gompper_kroll_ini(P, He *, /**/ T **);
int bending_juelicher_ini(P, He *, /**/ T **);
int bending_juelicher_xin_ini(P, He *, /**/ T **);
int bending_meyer_ini(P, He *, /**/ T **);
int bending_canham_ini(P, He *, /**/ T **);
int bending_gompper_xin_ini(P, He *, /**/ T **);
int bending_meyer_xin_ini(P, He *, /**/ T **);
int bending_meyer_ini(P, He *, /**/ T **);

#undef T
#undef P
