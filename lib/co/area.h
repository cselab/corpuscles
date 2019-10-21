typedef struct He He;
real he_area(He *, const real *, const real *, const real *);
int he_area_ver(He *, const real *, const real *, const real *,
                /**/ real *);
int he_area_tri(He *, const real *, const real *, const real *,
                /**/ real *);
int area_ver_voronoi(He *, const real *, const real *, const real *,
                     /**/ real *);
