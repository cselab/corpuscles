#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "he/vec.h"
#include "he/tri.h"

#include "he/dih.h"

int ddih_angle(real  a[3], real  b[3], real  c[3], real  d[3], /**/
               real da[3], real db[3], real dc[3], real dd[3]) {
    real n[3], k[3], bc[3];
    real e, An, Ak, cn, bn, bk, ck;

    tri_normal(a, b, c,   n);
    tri_normal(c, b, d,   k);

    vec_minus(c, b,   bc);
    e = vec_abs(bc);

    An = tri_area(a, b, c);
    Ak = tri_area(c, b, d);

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
