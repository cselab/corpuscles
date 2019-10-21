#define T F2Area
typedef struct Skel Skel;
typedef struct T T;
int f2_area_ini(real a, real k, Skel *, T **);
int f2_area_argv(char ***, Skel *, T **);
int f2_area_fin(T *);
real f2_area_energy(T *, Skel *, const real *, const real *);
int f2_area_force(T *, Skel *, const real *, const real *, real *, real *);
real f2_area_area(T *);

#undef T
