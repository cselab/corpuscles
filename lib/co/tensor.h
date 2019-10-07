struct Tensor {
    real *xx, *xy, *xz, *yy, *yz, *zz;
};

int tensor_ini(int, struct Tensor*);
int tensor_fin(struct Tensor*);
