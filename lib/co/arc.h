#define T Arc
typedef struct T T;
int arc_velocity_ini(int, real, real, real(*)(real, void *), void *, T **);
int arc_xy_ini(int, real, real, real(*)(real, void *),
               real(*)(real, void *), void *, T **);
int arc_points(T *, real **);
int arc_length(T *, real *);
int arc_fin(T *);

#undef T
