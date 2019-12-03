typedef struct He He;
int povray_mesh2(He *, const real *, const real *, const real *, FILE *);
int povray_tri_mesh2(He *, const real *, const real *, const real *, const real *color, FILE *);
int povray_ver_mesh2(He *, const real *, const real *, const real *, const real *color, FILE *);
int povray_lh_ver_mesh2(He *, const real *, const real *, const real *, real lo, real hi, const real *color, FILE *);
