#define T AlgIntegration
typedef struct T T;

enum {
    GAUSS15,
    GAUSS21,
    GAUSS31,
    GAUSS61,
    QAGS,
    QNG,
};

int alg_integration_ini(int, T **);
int alg_integration_fin(T *);
int alg_integration_apply(T *, real, real, real(*)(real, void *), void *,
                          /**/ real *);
int alg_integration_str2enum(const char *, int *);

#undef T
