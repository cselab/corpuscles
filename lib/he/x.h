extern const real pi;
extern int NV, NE, NT, NH;
extern int *T0, *T1, *T2;
extern int *D0, *D1, *D2, *D3;
extern real *XX, *YY, *ZZ;
extern real *VX, *VY, *VZ;
extern real *RR; /* sqrt(x^2 + y^2 + z^2) */
extern real *TH; /* acos(z/r) */

int ini(const char *path);
int fin();
int  nxt(int h);
int  flp(int h);

int  ver(int h);
int  edg(int h);
int  tri(int h);

int  hdg_ver(int v);
int  hdg_edg(int e);
int  hdg_tri(int t);

/* is bondary? */
int  bnd(int h);

real area();
real volume();

int off_write(const real*, const real*, const real*, const char*);

/* allocate and zero */
int RZERO(int, real**);

/* forces */
int f_area_ini(real a0, real K);
int f_area_fin();
real f_area_energy(const real *x, const real *y, const real *z);
int  f_area_force(const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);

int f_volume_ini(real v0, real K);
int f_volume_fin();
int f_volume_set_v(real v);
real f_volume_energy(const real *x, const real *y, const real *z);
int  f_volume_force(const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);

int f_harmonic_ini(real e0, real K);
int f_harmonic_fin();
real f_harmonic_energy(const real *x, const real *y, const real *z);
int  f_harmonic_force(const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);


int f_harmonic_ref_ini(real K, const real *x, const real *y, const real *z);
int f_harmonic_ref_fin();
real f_harmonic_ref_energy(const real *x, const real *y, const real *z);
int  f_harmonic_ref_force(const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);

int f_kantor_ini(real K);
int f_kantor_fin();
real f_kantor_energy(const real *x, const real *y, const real *z);
int  f_kantor_force(const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);

int f_gompper_ini(real K);
int f_gompper_fin();
real f_gompper_energy(const real *x, const real *y, const real *z);
int  f_gompper_force(const real *x, const real *y, const real *z, /**/ real *fx, real *fy, real *fz);
int f_gompper_energy_ver(real **p);
int f_gompper_area(real **p);
