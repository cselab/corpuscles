#define T Generation
typedef struct T T;
int generation_ini(He *, T **);
int generation_fin(T *);
int generation_refine(T *, int t, He *, real **, real **, real **);
int generation_invariant(T *, He *);
int generation_write(T *, He *, const real *, const real *, const real *,
                     FILE *);
int generation_read(FILE *, He **, real **, real **, real **, T **);

#undef T
