int linear_strain_tri(void *param, real (*F1)(void*, real, real),
                      real (*F2)(void*, real, real), real ax, real ay,
                      real bx, real by, real cx, real cy, real ux,
                      real uy, real vx, real vy, real wx, real wy,
                      real *dux, real *duy, real *dvx, real *dvy,
                      real *dwx, real *dwy, real *pI1, real *pI2);
