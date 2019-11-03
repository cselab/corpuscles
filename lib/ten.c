#include <stdio.h>

#include "real.h"
#include "co/err.h"
#include "inc/def.h"
#include "co/ten.h"

enum { X, Y, Z };
enum { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };
enum { SIZE = MAX_STRING_SIZE };

#define FMT_IN    CO_REAL_IN
#define FMT_OUT   CO_REAL_OUT

int
ten_matrix_ini(const real a[3 * 3], Ten * T)
{
    real *t;
    int i;

    t = T->t;
    for (i = 0; i < 3 * 3; i++)
        t[i] = a[i];
    return CO_OK;
}

int
ten_col_ini(const real a[3], const real b[3], const real c[3], Ten * T)
{
    real *t;

    t = T->t;
    t[XX] = a[X];
    t[XY] = b[X];
    t[XZ] = c[X];
    t[YX] = a[Y];
    t[YY] = b[Y];
    t[YZ] = c[Y];
    t[ZX] = a[Z];
    t[ZY] = b[Z];
    t[ZZ] = c[Z];
    return CO_OK;
}

int
ten_row_ini(const real a[3], const real b[3], const real c[3], Ten * T)
{
    real *t;

    t = T->t;
    t[XX] = a[X];
    t[XY] = a[Y];
    t[XZ] = a[Z];
    t[YX] = b[X];
    t[YY] = b[Y];
    t[YZ] = b[Z];
    t[ZX] = c[X];
    t[ZY] = c[Y];
    t[ZZ] = c[Z];
    return CO_OK;
}

int
ten_dyadic(const real a[3], const real b[3], /**/ Ten * T)
{
    real *t;

    t = T->t;
    t[XX] = a[X] * b[X];
    t[XY] = a[X] * b[Y];
    t[XZ] = a[X] * b[Z];
    t[YX] = a[Y] * b[X];
    t[YY] = a[Y] * b[Y];
    t[YZ] = a[Y] * b[Z];
    t[ZX] = a[Z] * b[X];
    t[ZY] = a[Z] * b[Y];
    t[ZZ] = a[Z] * b[Z];
    return CO_OK;
}

int
ten_plus(const Ten * P, const Ten * R, /**/ Ten * T)
{
    const real *p, *r;
    real *t;

    p = P->t;
    r = R->t;
    t = T->t;
    t[XX] = p[XX] + r[XX];
    t[XY] = p[XY] + r[XY];
    t[XZ] = p[XZ] + r[XZ];
    t[YX] = p[YX] + r[YX];
    t[YY] = p[YY] + r[YY];
    t[YZ] = p[YZ] + r[YZ];
    t[ZX] = p[ZX] + r[ZX];
    t[ZY] = p[ZY] + r[ZY];
    t[ZZ] = p[ZZ] + r[ZZ];
    return CO_OK;
}

int
ten_minus(const Ten * P, const Ten * R, /**/ Ten * T)
{
    const real *p, *r;
    real *t;

    p = P->t;
    r = R->t;
    t = T->t;
    t[XX] = p[XX] - r[XX];
    t[XY] = p[XY] - r[XY];
    t[XZ] = p[XZ] - r[XZ];
    t[YX] = p[YX] - r[YX];
    t[YY] = p[YY] - r[YY];
    t[YZ] = p[YZ] - r[YZ];
    t[ZX] = p[ZX] - r[ZX];
    t[ZY] = p[ZY] - r[ZY];
    t[ZZ] = p[ZZ] - r[ZZ];
    return CO_OK;
}

int
ten_scalar(const Ten * P, real s, /**/ Ten * T)
{
    const real *p;
    real *t;

    p = P->t;
    t = T->t;
    t[XX] = s * p[XX];
    t[XY] = s * p[XY];
    t[XZ] = s * p[XZ];
    t[YX] = s * p[YX];
    t[YY] = s * p[YY];
    t[YZ] = s * p[YZ];
    t[ZX] = s * p[ZX];
    t[ZY] = s * p[ZY];
    t[ZZ] = s * p[ZZ];
    return CO_OK;
}

int
ten_vec(const Ten * T, const real a[3], /**/ real b[3])
{
    const real *t;

    t = T->t;
    b[X] = t[XX] * a[X] + t[XY] * a[Y] + t[XZ] * a[Z];
    b[Y] = t[YX] * a[X] + t[YY] * a[Y] + t[YZ] * a[Z];
    b[Z] = t[ZX] * a[X] + t[ZY] * a[Y] + t[ZZ] * a[Z];
    return CO_OK;
}

int
vec_ten(const real a[3], const Ten * T, /**/ real b[3])
{
    const real *t;

    t = T->t;
    b[X] = a[X] * t[XX] + a[Y] * t[YX] + a[Z] * t[ZX];
    b[Y] = a[X] * t[XY] + a[Y] * t[YY] + a[Z] * t[ZY];
    b[Z] = a[X] * t[XZ] + a[Y] * t[YZ] + a[Z] * t[ZZ];
    return CO_OK;
}

int
ten_add(const Ten * R, /*io */ Ten * T)
{
    const real *r;
    real *t;

    r = R->t;
    t = T->t;
    t[XX] += r[XX];
    t[XY] += r[XY];
    t[XZ] += r[XZ];
    t[YX] += r[YX];
    t[YY] += r[YY];
    t[YZ] += r[YZ];
    t[ZX] += r[ZX];
    t[ZY] += r[ZY];
    t[ZZ] += r[ZZ];
    return CO_OK;
}

int
ten_sub(const Ten * R, /*io */ Ten * T)
{
    const real *r;
    real *t;

    r = R->t;
    t = T->t;
    t[XX] -= r[XX];
    t[XY] -= r[XY];
    t[XZ] -= r[XZ];
    t[YX] -= r[YX];
    t[YY] -= r[YY];
    t[YZ] -= r[YZ];
    t[ZX] -= r[ZX];
    t[ZY] -= r[ZY];
    t[ZZ] -= r[ZZ];
    return CO_OK;
}

int
ten_neg( /*io */ Ten * R)
{
    real *r;

    r = R->t;
    r[XX] = -r[XX];
    r[XY] = -r[XY];
    r[XZ] = -r[XZ];
    r[YX] = -r[YX];
    r[YY] = -r[YY];
    r[YZ] = -r[YZ];
    r[ZX] = -r[ZX];
    r[ZY] = -r[ZY];
    r[ZZ] = -r[ZZ];
    return CO_OK;
}

int
ten_mult_right(const Ten * R, /*io */ Ten * T)
{
    Ten A;
    const real *r;
    real *t, *a;

    r = R->t;
    t = T->t;
    a = A.t;
    ten_copy(T, &A);
    a[XX] = t[XZ] * r[ZX] + t[XY] * r[YX] + r[XX] * t[XX];
    a[XY] = t[XZ] * r[ZY] + t[XY] * r[YY] + t[XX] * r[XY];
    a[XZ] = t[XZ] * r[ZZ] + t[XY] * r[YZ] + t[XX] * r[XZ];
    a[YX] = t[YZ] * r[ZX] + r[YX] * t[YY] + r[XX] * t[YX];
    a[YY] = t[YZ] * r[ZY] + r[YY] * t[YY] + r[XY] * t[YX];
    a[YZ] = t[YZ] * r[ZZ] + t[YY] * r[YZ] + r[XZ] * t[YX];
    a[ZX] = r[ZX] * t[ZZ] + r[YX] * t[ZY] + r[XX] * t[ZX];
    a[ZY] = r[ZY] * t[ZZ] + r[YY] * t[ZY] + r[XY] * t[ZX];
    a[ZZ] = r[ZZ] * t[ZZ] + r[YZ] * t[ZY] + r[XZ] * t[ZX];
    ten_copy(&A, T);
    return CO_OK;
}

int
ten_mult_left(const Ten * R, /*io */ Ten * T)
{
    Ten A;
    const real *r;
    real *t, *a;

    r = R->t;
    t = T->t;
    a = A.t;
    ten_copy(T, &A);
    a[XX] = r[XZ] * t[ZX] + r[XY] * t[YX] + r[XX] * t[XX];
    a[XY] = r[XZ] * t[ZY] + r[XY] * t[YY] + r[XX] * t[XY];
    a[XZ] = r[XZ] * t[ZZ] + r[XY] * t[YZ] + r[XX] * t[XZ];
    a[YX] = r[YZ] * t[ZX] + t[YX] * r[YY] + t[XX] * r[YX];
    a[YY] = r[YZ] * t[ZY] + r[YY] * t[YY] + t[XY] * r[YX];
    a[YZ] = r[YZ] * t[ZZ] + r[YY] * t[YZ] + t[XZ] * r[YX];
    a[ZX] = t[ZX] * r[ZZ] + t[YX] * r[ZY] + t[XX] * r[ZX];
    a[ZY] = t[ZY] * r[ZZ] + t[YY] * r[ZY] + t[XY] * r[ZX];
    a[ZZ] = r[ZZ] * t[ZZ] + t[YZ] * r[ZY] + t[XZ] * r[ZX];
    ten_copy(&A, T);
    return CO_OK;
}

real
ten_trace(const Ten * T)
{
    const real *t;

    t = T->t;
    return t[XX] + t[YY] + t[ZZ];
}

real
ten_determinant(const Ten * T)
{
    const real *t;

    t = T->t;
    return
        t[XX] * (t[YY] * t[ZZ] - t[YZ] * t[ZY]) -
        t[XY] * (t[YX] * t[ZZ] - t[YZ] * t[ZX]) +
        t[XZ] * (t[YX] * t[ZY] - t[YY] * t[ZX]);
}

int
ten_scale(real s, Ten * T)
{
    real *t;

    t = T->t;
    t[XX] *= s;
    t[XY] *= s;
    t[XZ] *= s;
    t[YX] *= s;
    t[YY] *= s;
    t[YZ] *= s;
    t[ZX] *= s;
    t[ZY] *= s;
    t[ZZ] *= s;
    return CO_OK;
}

int
ten_axpy(real s, const Ten * P, /*io */ Ten * T)
{
    const real *p;
    real *t;

    p = P->t;
    t = T->t;
    t[XX] += s * p[XX];
    t[XY] += s * p[XY];
    t[XZ] += s * p[XZ];
    t[YX] += s * p[YX];
    t[YY] += s * p[YY];
    t[YZ] += s * p[YZ];
    t[ZX] += s * p[ZX];
    t[ZY] += s * p[ZY];
    t[ZZ] += s * p[ZZ];
    return CO_OK;
}

int
ten_copy(const Ten * P, /**/ Ten * T)
{
    const real *p;
    real *t;

    p = P->t;
    t = T->t;
    t[XX] = p[XX];
    t[XY] = p[XY];
    t[XZ] = p[XZ];
    t[YX] = p[YX];
    t[YY] = p[YY];
    t[YZ] = p[YZ];
    t[ZX] = p[ZX];
    t[ZY] = p[ZY];
    t[ZZ] = p[ZZ];
    return CO_OK;
}

int
ten_zero(Ten * T)
{
    real *t;

    t = T->t;
    t[XX] = t[XY] = t[XZ] = 0;
    t[YX] = t[YY] = t[YZ] = 0;
    t[ZX] = t[ZY] = t[ZZ] = 0;
    return CO_OK;
}

int
ten_one(Ten * T)
{
    real *t;

    t = T->t;
    t[XX] = 1;
    t[XY] = 0;
    t[XZ] = 0;
    t[YX] = 0;
    t[YY] = 1;
    t[YZ] = 0;
    t[ZX] = 0;
    t[ZY] = 0;
    t[ZZ] = 1;
    return CO_OK;
}

int
ten_sprintf(const Ten * T, const char *fmt0, /**/ char *s)
{
    int r;
    const real *t;
    char fmt[SIZE];

    t = T->t;
    r = snprintf(fmt, SIZE, "%s %s %s\n" "%s %s %s\n" "%s %s %s\n",
                 fmt0, fmt0, fmt0, fmt0, fmt0, fmt0, fmt0, fmt0, fmt0);
    if (r < 0)
        ERR(CO_IO, "snprintf failed for fmt0='%s'", fmt0);
    r = snprintf(s, SIZE, fmt,
                 t[XX], t[XY], t[XZ], t[YX], t[YY], t[YZ], t[ZX], t[ZY],
                 t[ZZ]);
    if (r < 0)
        ERR(CO_IO, "snprintf failed for fmt='%s'", fmt);
    return CO_OK;
}

int
ten_fprintf(const Ten * T, FILE * stream, const char *fmt0)
{
    char s[SIZE];

    if (ten_sprintf(T, fmt0, s) != CO_OK)
        ERR(CO_IO, "ten_sprintf failed");
    if (fputs(s, stream) == EOF)
        ERR(CO_IO, "fail to write to a stream");
    return CO_OK;
}

int
ten_printf(const Ten * T, const char *fmt0)
{
    if (ten_fprintf(T, stdout, fmt0) != CO_OK)
        ERR(CO_IO, "ten_fprintf failed");
    return CO_OK;
}

int
ten_line(const Ten * T)
{
    const real *t;

    t = T->t;
    printf(FMT_OUT " " FMT_OUT " " FMT_OUT " " FMT_OUT " " FMT_OUT " "
           FMT_OUT " " FMT_OUT " " FMT_OUT " " FMT_OUT, t[XX], t[XY],
           t[XZ], t[YX], t[YY], t[YZ], t[ZX], t[ZY], t[ZZ]);
    return CO_OK;
}

int
ten_fscanf(FILE * stream, Ten * T)
{
    real *t;
    int ok;

    t = T->t;
    ok = 1;
#define NXT(d) if (ok) ok = (fscanf(stream, FMT_IN, &t[d]) == 1)
    NXT(XX);
    NXT(XY);
    NXT(XZ);
    NXT(YX);
    NXT(YY);
    NXT(YZ);
    NXT(ZX);
    NXT(ZY);
    NXT(ZZ);
#undef NXT
    if (!ok)
        ERR(CO_IO, "fscanf failed");
    return CO_OK;
}

static int
nxt(const char *a, real * p)
{
    return sscanf(a, FMT_IN, p) == 1;
}

int
ten_argv(const char **pq[], /**/ Ten * T)
{
    const char **q;
    real *t;

    q = *pq;
    t = T->t;
#define NXT(d)                                                       \
    do {                                                                \
        if (*q == NULL) ERR(CO_IO, "not enough args");                  \
        if (!nxt(*q, &t[d]))                                            \
            ERR(CO_IO, "not a number '%s", *q);                         \
        q++;                                                            \
    } while (0);

    NXT(XX);
    NXT(XY);
    NXT(XZ);
    NXT(YX);
    NXT(YY);
    NXT(YZ);
    NXT(ZX);
    NXT(ZY);
    NXT(ZZ);
#undef NXT
    *pq = q;
    return CO_OK;
}
