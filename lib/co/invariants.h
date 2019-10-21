#define T Invariants

typedef struct He He;
typedef struct T T;

int invariants_ini(const char *off, /**/ T **);
int invariants_inif(FILE * off, /**/ T **);

int invariants_fin(T *);

int invariants_al(T *, const real *, const real *, const real *,
                  /**/ real **);
int invariants_be(T *, const real *, const real *, const real *,
                  /**/ real **);

#undef T
