#define T F2Glen
typedef struct Skel Skel;
typedef struct T T;
int f2_glen_ini(real l, real k, Skel *, T **);
int f2_glen_argv(char ***, Skel *, T **);
int f2_glen_fin(T *);
real f2_glen_energy(T *, Skel *, const real *, const real *);
int f2_glen_force(T *, Skel *, const real *, const real *, real *, real *);
real f2_glen_len(T *);

#undef T
