int min_ini(int type);
int min_fin();

int min_iterate();
int min_end();

real min_energy();
int min_position( /**/ real *, real *, real *);
int min_force( /**/ real *, real *, real *);

/* defined by the user */
real Energy(const real * xx, const real * yy, const real * zz);
void Force(const real * xx, const real * yy, const real * zz,
           /**/ real * fx, real * fy, real * fz);
