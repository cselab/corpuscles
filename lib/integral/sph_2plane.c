#include <stdio.h>
#include <tgmath.h>
#include "real.h"
#include "co/err.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/integral/sph_plane.h"
#include "co/integral/sph_2plane.h"

#define T Sph2Plane
#define FMT CO_REAL_OUT
#define PI (3.141592653589793)

struct T {
    real R;
    SphPlane *plane;
};

int
sph_2plane_ini(real R, T ** pq)
{
    T *q;

    MALLOC(1, &q);
    q->R = R;
    sph_plane_ini(R, &q->plane);
    *pq = q;
    return CO_OK;
}

int
sph_2plane_fin(T * q)
{
    sph_plane_fin(q->plane);
    FREE(q);
    return CO_OK;
}

int
sph_2plane_apply(T * q, real d0, real d1,
                 real(*E) (real, real, real, void *), void *param,
                 /**/ real * result)
{
    real R, a, b;

    R = q->R;
    if (d0 < 0)
        ERR(CO_NUM, "d0=" FMT "< 0", d0);
    if (d1 > R)
        ERR(CO_NUM, "d1=" FMT " > R=" FMT, d1, R);
    if (d1 < d0)
        ERR(CO_NUM, "d1=" FMT " < d0=" FMT, d1, d0);

    sph_plane_apply(q->plane, d0, E, param, &a);
    sph_plane_apply(q->plane, d1, E, param, &b);
    *result = b - a;
    return CO_OK;
}


int
sph_2plane_volume(T * q, real d0, real d1, /**/ real * result)
{
    real a, b;

    sph_plane_volume(q->plane, d1, &a);
    sph_plane_volume(q->plane, d0, &b);
    *result = b - a;
    return CO_OK;
}
