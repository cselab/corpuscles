#define T F2BendMin

typedef struct Skel Skel;
typedef struct T T;

int f2_bend_min_ini(real k, Skel *, T **);
int f2_bend_min_argv(char ***, Skel *, T **);
int f2_bend_min_fin(T *);

real f2_bend_min_energy(T *, Skel *, const real *, const real *);
int f2_bend_min_force(T *, Skel *, const real *, const real *, real *,
                      real *);

#undef T
