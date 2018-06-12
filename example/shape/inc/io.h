static void write(real *x, real *y, real *z,
                  real *vx, real *vy, real *vz,
		  real *fx, real *fy, real *fz) {

  real *queue[] = {x, y, z, vx, vy, vz, fx, fy, fz, NULL};
  punto_fwrite(NV, queue, stdout);  
  
}
