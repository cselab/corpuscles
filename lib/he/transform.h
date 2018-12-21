int transform_centroid(int, const real*, const real*, const real*,
                       /**/ real[3]);

int transform_rotx(real, int /*io*/ real*, real*, real*);
int transform_roty(real, int /*io*/ real*, real*, real*);
int transform_rotz(real, int /*io*/ real*, real*, real*);
int transform_rot(const real[3], int /*io*/ real*, real*, real*);

int transform_tranx(real, int /*io*/ real*, real*, real*);
int transform_trany(real, int /*io*/ real*, real*, real*);
int transform_tranz(real, int /*io*/ real*, real*, real*);
int transform_tran(const real[3], int /*io*/ real*, real*, real*);

int transform_scalx(real, int /*io*/ real*, real*, real*);
int transform_scaly(real, int /*io*/ real*, real*, real*);
int transform_scalz(real, int /*io*/ real*, real*, real*);
int transform_scal(const real[3], int /*io*/ real*, real*, real*);
