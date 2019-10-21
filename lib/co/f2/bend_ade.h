#define T F2BendAde
typedef struct Skel Skel;
typedef struct T T;
int f2_bend_ade_ini(real k, real H0, Skel *, T **);
int f2_bend_ade_argv(char ***, Skel *, T **);
int f2_bend_ade_fin(T *);
real f2_bend_ade_energy(T *, Skel *, const real *, const real *);
int f2_bend_ade_force(T *, Skel *, const real *, const real *, real *,
                      real *);
#undef T
