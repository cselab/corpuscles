#include <stdio.h>
#include <math.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"

#include "he/dih.h"
#include "he/edg.h"

int ddih_angle(const real a[3], const real b[3], const real c[3], const real d[3], /**/
               real da[3], real db[3], real dc[3], real dd[3]) {
    real n[3], k[3];
    real e, An, Ak, cn, bn, bk, ck;

    tri_normal(a, b, c,   n);
    tri_normal(c, b, d,   k);

    e = edg_abs(c, b);
    if (e == 0) ERR(HE_NUM, "e == 0");

    An = tri_area(a, b, c);
    if (An == 0) ERR(HE_NUM, "An == 0");

    Ak = tri_area(c, b, d);
    if (Ak == 0) ERR(HE_NUM, "Ak == 0");

    vec_scalar(n, e/(2*An),   da);
    vec_scalar(k, e/(2*Ak),   dd);

    cn = tri_cot(b, c, a);
    bn = tri_cot(a, b, c);

    bk = tri_cot(c, b, d);
    ck = tri_cot(d, c, b);

    vec_linear_combination(-bn/e, n, -bk/e, k,    dc);
    vec_linear_combination(-cn/e, n, -ck/e, k,    db);

    return HE_OK;
}

int ddih_cos(const real a[3], const real b[3], const real c[3], const real d[3], /**/
             real da[3], real db[3], real dc[3], real dd[3]) {
    int status;
    real ang, coef, da0[3], db0[3], dc0[3], dd0[3];

    status = ddih_angle(a, b, c, d, /**/ da0, db0, dc0, dd0);
    if (status != HE_OK)
        ERR(HE_NUM, "ddih_angle failed");

    ang  =  dih_angle(a, b, c, d);
    coef = -sin(ang);

    vec_scalar(da0, coef,   da);
    vec_scalar(db0, coef,   db);
    vec_scalar(dc0, coef,   dc);
    vec_scalar(dd0, coef,   dd);

    return HE_OK;
}
