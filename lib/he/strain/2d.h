int strain_2d(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
              real bx, real by, real cx, real cy, real ux, real uy,
              real wx, real wy, real *dvx, real *dvy, real *dux,
              real *duy, real *dwx, real *dwy, real *pI1, real *pI2,
              real *pA);
