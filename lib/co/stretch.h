#define T Stretch

typedef struct T T;
typedef struct He He;

int stretch_argv(char ***, He *, real *, real *, real *, /**/ T **);
int stretch_help(const char **);
int stretch_fin(T *);

int stretch_force(T *, const real *, const real *, const real *,    /*io */
                  real *, real *, real *);
int stretch_constrain(T *, const real *, const real *, const real *,
                      /*io */ real *, real *, real *);
int stretch_n(T *);
real stretch_f(T *);

#undef T
