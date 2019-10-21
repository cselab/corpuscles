#define T F2BendSc
typedef struct Skel Skel;
typedef struct T T;
int f2_bend_sc_ini(real Kad, real DA0D, Skel *, T **);
int f2_bend_sc_argv(char ***, Skel *, T **);
int f2_bend_sc_fin(T *);
real f2_bend_sc_energy(T *, Skel *, const real *, const real *);
int f2_bend_sc_force(T *, Skel *, const real *, const real *, real *,
                     real *);
#undef T
