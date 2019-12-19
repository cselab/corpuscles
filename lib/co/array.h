int array_axpy3(int, real, const real[], const real[], const real[],
                /**/ real[], real[], real[]);
int array_axpy(int, real, const real[], /**/ real[]);
int array_copy3(int, const real[], const real[], const real[], real[],
                real[], real[]);
int array_copy(int, const real[], real[]);
int array_fwrite(FILE *, int, const real[]);
int array_fwrite(FILE *, int, const real[]);
int array_multiply3(int, const real[], real[], real[], real[]);
int array_multiply(int, const real[], real[]);
int array_negative(int, const real[], real[]);
int array_neg(int, real[]);
int array_one(int, real[]);
int array_scale3(int, real, real[], real[], real[]);
int array_scale(int, real, real[]);
int array_sum(int, const real[], const real[], real[]);
int array_zero3(int, real[], real[], real[]);
int array_zero(int, real[]);
real array_diff3(int, const real[], const real[], const real[],
                 const real[], const real[], const real[]);
real array_diff(int, const real[], const real[]);
real array_dot(int, const real[], const real[]);
real array_l2_3d(int, const real[], const real[], const real[],
                 const real[], const real[], const real[]);
real array_l2(int, const real[], const real[]);
real array_max3(int, const real[], const real[], const real[]);
real array_max(int, const real[]);
real array_mean(int, const real[]);
real array_min3(int, const real[], const real[], const real[]);
real array_min(int, const real[]);
real array_msq_3d(int, const real[], const real[], const real[]);
real array_msq(int, const real[]);
real array_norm(int, const real[]);
