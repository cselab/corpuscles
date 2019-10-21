#define T F2Len

typedef struct Skel Skel;
typedef struct T T;
int f2_len_ini(FILE *, real Ka, real a3, real a4, T **);
int f2_len_argv(char ***, Skel *, T **);
int f2_len_fin(T *);
real f2_len_energy(T *, Skel *, const real *, const real *);
int f2_len_force(T *, Skel *, const real *, const real *, real *, real *);

#undef T
