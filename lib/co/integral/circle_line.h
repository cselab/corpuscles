#define T CircleLine

typedef struct T T;
int circle_line_ini(real d, T **);
int circle_line_fin(T *);
int circle_line_apply(T *, real r, real(*)(real x, real y, void *), void *,
                      /**/ real *);
int circle_line_volume(T *, real d, /**/ real *);

#undef T
