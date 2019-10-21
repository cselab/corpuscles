#define T Skel
typedef struct T T;
int skel_close_ini(int nv, T **);
int skel_open_ini(int nv, T **);
int skel_circle_ini(int nv, /**/ real * length, real ** x, real ** y,
                    T **);
int skel_ellipse_ini(int nv, real b, /**/ real * length, real ** x,
                     real ** y, T **);
int skel_rbc_ini(int nv, /**/ real * length, real ** x, real ** y, T **);
int skel_read(FILE *, /**/ real **, real **, T **);
int skel_fin(T *);
int skel_xy_fin(real *, real *, T *);
int skel_write(T *, const real *, const real *, FILE *);
int skel_punto_write(T *, const real *, const real *, FILE *);
int skel_off_write(int, const real *, const real *, FILE *);
int skel_edg_write(T *, const real *, const real *, FILE *);
int skel_vtk_write(T *, const real *, const real *, FILE *);
int skel_edg_lh_write(T *, const real *, const real *, real lo, real hi,
                      const real *, FILE *);
int skel_edg_color_write(T *, const real *, const real *, const real *,
                         FILE *);
int skel_nv(T *);
int skel_ne(T *);
int skel_bnd(T *, int);
int skel_close(T *);
int skel_edg_ij(T *, int, /**/ int *, int *);
int skel_ver_ijk(T *, int, /**/ int *, int *, int *);

#undef T
