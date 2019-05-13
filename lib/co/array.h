real array_min(int, const real[]);
real array_max(int, const real[]);
real array_dot(int, const real[], const real[]);
real array_mean(int, const real[]);

int array_zero(int, real[]);
int array_one(int, real[]);
int array_zero3(int, real[], real[], real[]);

int array_axpy(int, real, const real[], /**/ real[]);
int array_axpy3(int, real, const real[], const real[], const real[], /**/ real[], real[], real[]);

int array_copy(int, const real[], real[]);
int array_negative(int, const real[], real[]);
