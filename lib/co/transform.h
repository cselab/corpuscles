typedef struct He He;

int transform_centroid(He *, const real *, const real *, const real *,
                       /**/ real[3]);

int transform_rotx(real rad, int, /*io */ real *, real *, real *);
int transform_roty(real rad, int, /*io */ real *, real *, real *);
int transform_rotz(real rad, int, /*io */ real *, real *, real *);

int transform_tranx(real, int, /*io */ real *, real *, real *);
int transform_trany(real, int, /*io */ real *, real *, real *);
int transform_tranz(real, int, /*io */ real *, real *, real *);
int transform_tran(const real[3], int, /*io */ real *, real *, real *);

int transform_scalx(real, int, /*io */ real *, real *, real *);
int transform_scaly(real, int, /*io */ real *, real *, real *);
int transform_scalz(real, int, /*io */ real *, real *, real *);
int transform_scal(const real[3], int, /*io */ real *, real *, real *);
