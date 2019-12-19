#include <stdio.h>

#include "real.h"

#include "co/argv.h"
#include "co/array.h"
#include "co/err.h"
#include "co/he.h"
#include "co/macro.h"
#include "co/memory.h"
#include "co/strain/3d.h"
#include "co/strain.h"
#include "co/sum.h"
#include "co/tri.h"
#include "co/util.h"
#include "co/vec.h"
#include "co/y.h"

#include "inc/def.h"

#include "co/f/strain.h"

#define T HeFStrain
#define OUT CO_REAL_OUT

#define BEGIN                                                   \
for (t = 0; t < nt; t++) {                                      \
    he_tri_ijk(he, t, /**/ &i, &j, &k);                         \
    get3(q->x, q->y, q->z, i, j, k, /**/ a0, b0, c0);           \
    get3(x, y, z, i, j, /**/ k, a, b, c);
#define END }

static const real EPS = 1e-8;

struct T {
    He *he;
    real *x, *y, *z, *eng, *ea, *eb, *I1, *I2;
    real *I1t, *I2t;     /* on triangles */
    Strain *strain;
};

static int
get3(const real * x, const real * y, const real * z,
     int i, int j, int k, /**/ real a[3], real b[3], real c[3])
{
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    return CO_OK;
}

int
he_f_strain_ini(const char *off, const char *name, StrainParam param,
		T ** pq)
{
    T *q;
    int nv, nt, status;
    He *he;
    real *x, *y, *z;

    MALLOC(1, &q);

    status = y_ini(off, &he, &x, &y, &z);
    if (status != CO_OK)
	ERR(CO_IO, "y_ini failed");

    nv = he_nv(he);
    nt = he_nt(he);

    MALLOC(nv, &q->eng);
    MALLOC(nv, &q->I1);
    MALLOC(nv, &q->I2);
    MALLOC(nt, &q->I1t);
    MALLOC(nt, &q->I2t);
    MALLOC(nt, &q->ea);
    MALLOC(nt, &q->eb);
    strain_ini(name, param, &q->strain);
    q->he = he;
    q->x = x;
    q->y = y;
    q->z = z;
    *pq = q;
    return CO_OK;
}

int
he_f_strain_argv(char ***p, __UNUSED He * he, T ** pq)
{
#define PAR(f)                                       \
    do {                                                \
    if ((status = argv_real(p, &param.f)) != CO_OK)     \
	return status;                                  \
    } while (0)

    int status;
    StrainParam param;
    char off[MAX_STRING_SIZE], name[MAX_STRING_SIZE];

    if ((status = argv_str(p, off)) != CO_OK)
	return status;

    if ((status = argv_str(p, name)) != CO_OK)
	return status;

    if (util_eq(name, "linear")) {
	PAR(Ka);
	PAR(mu);
    } else if (util_eq(name, "lim")) {
	PAR(Ka);
	PAR(mu);
	PAR(a3);
	PAR(a4);
	PAR(b1);
	PAR(b2);
    } else
	ERR(CO_IO, "unknown strain model '%s'", name);

    status = he_f_strain_ini(off, name, param, pq);
    return status;
#undef PAR
}

int
he_f_strain_fin(T * q)
{
    y_fin(q->he, q->x, q->y, q->z);
    strain_fin(q->strain);
    FREE(q->eng);
    FREE(q->ea);
    FREE(q->eb);
    FREE(q->I1);
    FREE(q->I2);
    FREE(q->I1t);
    FREE(q->I2t);
    FREE(q);
    return CO_OK;
}

static int
small(const real a[3])
{
    return vec_abs(a) < EPS;
}

static int
assert_force(const real a[3], const real b[3], const real c[3],
	     const real da[3], const real db[3], const real dc[3])
{
    /* check force and  torque */
    real m[3], f[3], t[3], ma[3], mb[3], mc[3];
    real ta[3], tb[3], tc[3];

    tri_center(a, b, c, /**/ m);
    vec_minus(a, m, /**/ ma);
    vec_minus(b, m, /**/ mb);
    vec_minus(c, m, /**/ mc);
    vec_mean3(da, db, dc, /**/ f);

    vec_cross(ma, da, /**/ ta);
    vec_cross(mb, db, /**/ tb);
    vec_cross(mc, dc, /**/ tc);
    vec_mean3(ta, tb, tc, /**/ t);

    if (!small(f) || !small(t)) {
	MSG("bad triangle in strain");
	MSG("a, b, c, f, t:");
	vec_fprintf(a, stderr, OUT);
	vec_fprintf(b, stderr, OUT);
	vec_fprintf(c, stderr, OUT);
	vec_fprintf(f, stderr, OUT);
	vec_fprintf(t, stderr, OUT);
	return 0;
    } else
	return 1;
}

int
he_f_strain_force(T * q, __UNUSED He * he0, const real * x, const real * y,
		  const real * z, /**/ real * fx, real * fy, real * fz)
{
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3], da[3], db[3], dc[3];
    int nv, nt, t;
    int i, j, k;
    He *he;

    he = q->he;
    nv = he_nv(he);
    nt = he_nt(he);

    if (nv != he_nv(he0))
	ERR(CO_INDEX, "nv=%d != he_nv(he0)=%d", nv, he_nv(he0));
    if (nt != he_nt(he0))
	ERR(CO_INDEX, "nt=%d != he_nt(he0)=%d", nt, he_nt(he0));

    BEGIN {
	strain_force(q->strain, a0, b0, c0, a, b, c, /**/ da, db, dc);
	if (!assert_force(a, b, c, da, db, dc))
	    ERR(CO_NUM, "bad forces in triangle: %d [%d %d %d]", t, i, j,
		k);
	vec_append(da, i, /**/ fx, fy, fz);
	vec_append(db, j, /**/ fx, fy, fz);
	vec_append(dc, k, /**/ fx, fy, fz);
    }
    END;
    return CO_OK;
}

real
he_f_strain_energy(T * q, He * he0, const real * x, const real * y, const real * z)
{
    real a0[3], b0[3], c0[3];
    real a[3], b[3], c[3];
    int nv, nt, t;
    int i, j, k;
    real ea;
    real eb;
    He *he;

    he = q->he;
    nv = he_nv(he);
    nt = he_nt(he);
    if (nv != he_nv(he0))
	ERR(CO_INDEX, "nv=%d != he_nv(he0)=%d", nv, he_nv(he0));
    if (nt != he_nt(he0))
	ERR(CO_INDEX, "nt=%d != he_nt(he0)=%d", nt, he_nt(he0));
    array_zero(nv, q->eng);
    array_zero(nv, q->ea);
    array_zero(nv, q->eb);
    BEGIN {
	strain_energy_ab(q->strain, a0, b0, c0, a, b, c, &ea, &eb);
	q->ea[i] += ea / 3;
	q->ea[j] += ea / 3;
	q->ea[k] += ea / 3;
	q->eb[i] += eb / 3;
	q->eb[j] += eb / 3;
	q->eb[k] += eb / 3;
    }
    END;
    return sum_array(nv, q->eb);
}
