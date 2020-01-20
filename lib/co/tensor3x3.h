struct Tensor3x3 {
    real *d[3*3];
};

int tensor3x3_ini(int, struct Tensor3x3 *);
int tensor3x3_fin(struct Tensor3x3 *);
int tensor3x3_zero(int, struct Tensor3x3 *);
int tensor3x3_scale(int, real, struct Tensor3x3 *);
int tensor3x3_vector(int, real, const real *, const real *, const real *,
		     struct Tensor3x3 *, /**/ real *, real *, real *);
