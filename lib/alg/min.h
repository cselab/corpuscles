#define T AlgMin

typedef struct T T;

enum { CONJUGATE_FR, CONJUGATE_PR, VECTOR_BFGS2, VECTOR_BFGS,
    STEEPEST_DESCENT
};

typedef real(*AlgMinF) (int n, const real * xx, const real * yy,
                        const real * zz, void *param);
typedef void (*AlgMinDF)(int n, const real * xx, const real * yy,
                         const real * zz, void *param, /**/ real * fx,
                         real * fy, real * fz);
int alg_min_ini(int type, AlgMinF, AlgMinDF, void *param, int n, real * xx,
                real * yy, real * zz, /**/ T **);
int alg_min_fin(T *);

int alg_min_iterate(T *);

int alg_min_force(T *, /**/ real **, real **, real **);
int alg_min_position(T *, /**/ real **, real **, real **);
real alg_min_energy(T *);

int alg_min_end(T *);

#undef T
