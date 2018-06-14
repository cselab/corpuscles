#include <stdio.h>

#include "real.h"
#include "he/err.h"
#include "inc/def.h"
#include "he/ten.h"

enum {X, Y, Z};
enum {XX, XY, XZ,   YX, YY, YZ,   ZX, ZY, ZZ};
enum {SIZE = MAX_STRING_SIZE};
#define FMT_IN   XE_REAL_IN

void ten_dyadic(const real a[3], const real b[3], /**/ Ten *T) {
    real *t;
    t = T->t;
    t[XX] = a[X]*b[X]; t[XY] = a[X]*b[Y]; t[XZ] = a[X]*b[Z];
    t[YX] = a[Y]*b[X]; t[YY] = a[Y]*b[Y]; t[YZ] = a[Y]*b[Z];
    t[ZX] = a[Z]*b[X]; t[ZY] = a[Z]*b[Y]; t[ZZ] = a[Z]*b[Z];
}

void ten_plus(const Ten *P, const Ten *R, /**/ Ten *T) {
    const real *p, *r;
    real *t;
    p = P->t; r = R->t; t = T->t;
    t[XX] = p[XX] + r[XX]; t[XY] = p[XY] + r[XY]; t[XZ] = p[XZ] + r[XZ];
    t[YX] = p[YX] + r[YX]; t[YY] = p[YY] + r[YY]; t[YZ] = p[YZ] + r[YZ];
    t[ZX] = p[ZX] + r[ZX]; t[ZY] = p[ZY] + r[ZY]; t[ZZ] = p[ZZ] + r[ZZ];
}

void ten_minus(const Ten *P, const Ten *R, /**/ Ten *T) {
    const real *p, *r;
    real *t;
    p = P->t; r = R->t; t = T->t;
    t[XX] = p[XX] - r[XX]; t[XY] = p[XY] - r[XY]; t[XZ] = p[XZ] - r[XZ];
    t[YX] = p[YX] - r[YX]; t[YY] = p[YY] - r[YY]; t[YZ] = p[YZ] - r[YZ];
    t[ZX] = p[ZX] - r[ZX]; t[ZY] = p[ZY] - r[ZY]; t[ZZ] = p[ZZ] - r[ZZ];
}

void ten_scalar(const Ten *P, real s, /**/ Ten *T) {
    const real *p;
    real *t;
    p = P->t; t = T->t;
    t[XX] = s*p[XX]; t[XY] = s*p[XY]; t[XZ] = s*p[XZ];
    t[YX] = s*p[YX]; t[YY] = s*p[YY]; t[YZ] = s*p[YZ];
    t[ZX] = s*p[ZX]; t[ZY] = s*p[ZY]; t[ZZ] = s*p[ZZ];
}

void ten_vec(const Ten *T, const real a[3], /**/ real b[3]) {
    const real *t;
    t = T->t;
    b[X] = t[XX]*a[X] + t[XY]*a[Y] + t[XZ]*a[Z];
    b[Y] = t[YX]*a[X] + t[YY]*a[Y] + t[YZ]*a[Z];
    b[Z] = t[ZX]*a[X] + t[ZY]*a[Y] + t[ZZ]*a[Z];
}

void ten_add(const Ten *R, /*io*/ Ten *T) {
    const real *r;
    real *t;
    r = R->t; t = T->t;
    t[XX] += r[XX]; t[XY] += r[XY]; t[XZ] += r[XZ];
    t[YX] += r[YX]; t[YY] += r[YY]; t[YZ] += r[YZ];
    t[ZX] += r[ZX]; t[ZY] += r[ZY]; t[ZZ] += r[ZZ];
}

void ten_axpy(real s, const Ten *P, /*io*/ Ten *T) {
    const real *p;
    real *t;
    p = P->t; t = T->t;
    t[XX] += s*p[XX]; t[XY] += s*p[XY]; t[XZ] += s*p[XZ];
    t[YX] += s*p[YX]; t[YY] += s*p[YY]; t[YZ] += s*p[YZ];
    t[ZX] += s*p[ZX]; t[ZY] += s*p[ZY]; t[ZZ] += s*p[ZZ];
}

void ten_copy(const Ten *P, /**/ Ten *T) {
    const real *p;
    real *t;
    p = P->t; t = T->t;
    t[XX] = p[XX]; t[XY] = p[XY]; t[XZ] = p[XZ];
    t[YX] = p[YX]; t[YY] = p[YY]; t[YZ] = p[YZ];
    t[ZX] = p[ZX]; t[ZY] = p[ZY]; t[ZZ] = p[ZZ];
}

void ten_zero(Ten *T) {
    real *t;
    t = T->t;
    t[XX] = t[XY] = t[XZ] = 0;
    t[YX] = t[YY] = t[YZ] = 0;
    t[ZX] = t[ZY] = t[ZZ] = 0;
}

void ten_one(Ten *T) {
    real *t;
    t = T->t;
    t[XX] = 1; t[XY] = 0; t[XZ] = 0;
    t[YX] = 0; t[YY] = 1; t[YZ] = 0;
    t[ZX] = 0; t[ZY] = 0; t[ZZ] = 1;
}

int ten_sprintf(const Ten *T, const char *fmt0, /**/ char *s) {
    int r;
    const real *t;
    char fmt[SIZE];
    t = T->t;
    r = snprintf(fmt, SIZE, "%s %s %s\n" "%s %s %s\n" "%s %s %s\n",
                 fmt0, fmt0, fmt0, fmt0, fmt0, fmt0, fmt0, fmt0, fmt0);
    if (r < 0)
        ERR(HE_IO, "snprintf failed for fmt0='%s'", fmt0);
    r = snprintf(s, SIZE, fmt,
                 t[XX], t[XY], t[XZ], t[YX], t[YY], t[YZ], t[ZX], t[ZY], t[ZZ]);
    if (r < 0)
        ERR(HE_IO, "snprintf failed for fmt='%s'", fmt);
    return HE_OK;
}

int ten_fprintf(const Ten *T, FILE *stream, const char *fmt0) {
    char s[SIZE];
    if (ten_sprintf(T, fmt0, s) != HE_OK)
        ERR(HE_IO, "ten_sprintf failed");
    if (fputs(s, stream) == EOF)
        ERR(HE_IO, "fail to write to a stream");
    return HE_OK;
}

int ten_printf(const Ten *T, const char *fmt0) {
    if (ten_fprintf(T, stdout, fmt0) != HE_OK)
        ERR(HE_IO, "ten_fprintf failed");
    return HE_OK;
}

int ten_fscanf(FILE *stream, Ten *T) {
    real *t;
    int ok;
    t = T->t;
    ok = 1;
#   define NXT(d) if (ok) ok = (fscanf(stream, FMT_IN, &t[d]) == 1)
    NXT(XX); NXT(XY); NXT(XZ);
    NXT(YX); NXT(YY); NXT(YZ);
    NXT(ZX); NXT(ZY); NXT(ZZ);
#   undef NXT
    if (!ok) ERR(HE_IO, "fscanf failed");
    return HE_OK;
}

static int nxt(const char *a, real *p) {
    return sscanf(a, FMT_IN, p) == 1;
}
int ten_argv(const char **pq[], /**/ Ten *T) {
    const char **q;
    real *t;
    q = *pq;
    t = T->t;
#   define NXT(d)                                                       \
    do {                                                                \
        if (*q == NULL) ERR(HE_IO, "not enough args");                   \
        if (!nxt(*q, &t[d]))                                            \
            ERR(HE_IO, "not a number '%s", *q);                         \
        q++;                                                            \
    } while (0);

    NXT(XX); NXT(XY); NXT(XZ);
    NXT(YX); NXT(YY); NXT(YZ);
    NXT(ZX); NXT(ZY); NXT(ZZ);
#   undef NXT
    *pq = q;
    return HE_OK;
}
