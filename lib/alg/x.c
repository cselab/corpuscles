#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "alg/min.h"
#include "alg/x.h"

extern int NV;
extern real *XX, *YY, *ZZ;      /* defined in xe */
static AlgMin *min;

static real
Energy0(int __UNUSED n, const real * xx, const real * yy, const real * zz,
        void __UNUSED * param)
{
    return Energy(xx, yy, zz);
}

static void
Force0(__UNUSED int n,
       const real * xx, const real * yy, const real * zz,
       void __UNUSED * param, real * fx, real * fy, real * fz)
{
    Force(xx, yy, zz, /**/ fx, fy, fz);
}

int
min_ini(int type)
{
    if (NV < 0)
        ER("NV < 0");
    alg_min_ini(type, Energy0, Force0, NULL, NV, XX, YY, ZZ, &min);
    return CO_OK;
}

int
min_fin()
{
    alg_min_fin(min);
    return CO_OK;
}

int
min_iterate()
{
    return alg_min_iterate(min);
}

real
min_energy()
{
    return alg_min_energy(min);
}

int
min_end()
{
    return alg_min_end(min);
}

int
min_force(real * x, real * y, real * z)
{
    int i;
    real *fx, *fy, *fz;

    alg_min_force(min, /**/ &fx, &fy, &fz);
    for (i = 0; i < NV; i++) {
        x[i] = fx[i];
        y[i] = fy[i];
        z[i] = fz[i];
    }
    return CO_OK;
}

int
min_position(real * x, real * y, real * z)
{
    int i;
    real *fx, *fy, *fz;

    alg_min_position(min, /**/ &fx, &fy, &fz);
    for (i = 0; i < NV; i++) {
        x[i] = fx[i];
        y[i] = fy[i];
        z[i] = fz[i];
    }
    return CO_OK;
}
