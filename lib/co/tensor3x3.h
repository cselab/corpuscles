struct Tensor {
    real *d[3*3];
};

int tensor3x3_ini(int, struct Tensor *);
int tensor3x3_fin(struct Tensor *);
int tensor3x3_vector(int, real, const real *, const real *, const real *,
		     struct Tensor *, /**/ real *, real *, real *);
