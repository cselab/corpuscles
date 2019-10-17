struct Tensor {
    real *xx, *xy, *xz, *yy, *yz, *zz;
};

int tensor_ini(int, struct Tensor*);
int tensor_fin(struct Tensor*);
int tensor_vector(int, real, const real*, const real*, const real*, struct Tensor*, /**/ real*, real*, real*);
