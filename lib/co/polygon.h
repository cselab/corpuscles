#define T Polygon

typedef struct T T;
typedef struct Skel Skel;

int polygon_ini(real lo, real hi, real size, T **);
int polygon_fin(T *);
int polygon_update(T *, Skel *, const real * x, const real * y);
int polygon_inside(T *, real, real);
int polygon_inside_fast(T *, real, real);

#undef T
