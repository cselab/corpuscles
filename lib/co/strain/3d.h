int strain_force_3d(void *param,
                    real(*F) (void *, real, real), real(*F1) (void *, real,
                                                              real),
                    real(*F2) (void *, real, real), const real a0[3],
                    const real b0[3], const real c0[3], const real a[3],
                    const real b[3], const real c[3], /**/ real da[3],
                    real db[3], real dc[3]);

int strain_energy_3d(void *param, real(*F) (void *, real, real),
                     const real a0[3], const real b0[3], const real c0[3],
                     const real a[3], const real b[3], const real c[3],
                     real * p_eng);

int strain_energy_3d_ab(void *param, int(*G) (void *, real, real, real *, real *),
			const real a0[3], const real b0[3], const real c0[3],
			const real a[3], const real b[3], const real c[3],
			real * ea, real *eb);

int strain_invariants(const real a0[3], const real b0[3], const real c0[3],
                      const real a[3], const real b[3], const real c[3],
                      real * I1, real * I2);
