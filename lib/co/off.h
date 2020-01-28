#define T Off
typedef struct He He;
typedef struct T T;
int off_ini(const char *, T **);
int off_inif(FILE *, T **);
int off_fin(T *);
int off_nt(T *);
int off_nv(T *);
int off_ver(T *, real **);
int off_tri(T *, int **);
int off_xyz(T *, /**/ real *, real *, real *);
int off_fwrite(T *, const real *, const real *, const real *, FILE *);
int off_write0(T *, const real *, const real *, const real *,
               /**/ const char *);
int off_tri_fwrite(T *, const int *tri, FILE *);
int off_tri_write(T *, const int *tri, /**/ const char *);
int off_he_fwrite(T *, He *, FILE *);
int off_he_write(T *, He *, /**/ const char *);
int off_xyz_tri_fwrite(int, const real * xyz, int, const int *tri, FILE *);
int off_he_xyz_fwrite(He *, const real *, const real *, const real *,
                      FILE *);
int off_he_xyz_write(He *, const real *, const real *, const real *,
                     const char *);
int off_lh_ver_fwrite(He *, const real *, const real *, const real *,
                       real lo, real hi, const real *, FILE *);
int boff_fwrite(He *, const real *, const real *, const real *, FILE *);
int boff_lh_tri_fwrite(He *, const real *, const real *, const real *,
                       real lo, real hi, const real *, FILE *);
int boff_tri_fwrite(He *, const real *, const real *, const real *,
                    const real *, FILE *);
int boff_lh_ver_fwrite(He *, const real *, const real *, const real *,
                       real lo, real hi, const real *, FILE *);
int boff_ver_fwrite(He *, const real *, const real *, const real *,
                    const real *, FILE *);
int off_ver_fwrite(He *, const real *, const real *, const real *,
                    const real *, FILE *);
int boff_vect_fwrite(He *, const real *, const real *, const real *,
                     const real *, const real *, const real *, FILE *);
int boff_lh_point_fwrite(He *, const real *, const real *, const real *,
                         real lo, real hi, const real *, FILE *);
int boff_point_fwrite(He *, const real *, const real *, const real *,
                      const real *, FILE *);
#undef T
