#include <stdio.h>

#include "real.h"

#include "co/argv.h"
#include "co/memory.h"
#include "co/err.h"
#include "co/he.h"
#include "co/vec.h"
#include "co/tri.h"
#include "co/dtri.h"

#include "co/f/gompper.h"

#define T HeFGompper

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  hdg_tri(e) he_hdg_tri(he, (e))
#define  bnd(h)     he_bnd(he, (h))

static const real pi = 3.141592653589793115997964;

struct T {
	int nv, ne, nt;
	real Kb, C0, Kad, DA0D;
	int *T0, *T1, *T2;
	real *l2, *t;
	real *lbx, *lby, *lbz;
	real *normx, *normy, *normz;
	real *curva_mean, *curva_gauss;
	real *energy, *area;
	real eng_bend, eng_ad;
};

static void
get_edg(int i, int j, const real * x, const real * y, const real * z,
		/**/ real r[3])
{
	real a[3], b[3];

	vec_get(i, x, y, z, a);
	vec_get(j, x, y, z, b);
	vec_minus(a, b, r);
}

static int
get_ijk(int t, He * he, /**/ int *pi, int *pj, int *pk)
{
	int h, n, nn, i, j, k;

	h = hdg_tri(t);
	n = nxt(h);
	nn = nxt(n);
	i = ver(h);
	j = ver(n);
	k = ver(nn);
	*pi = i;
	*pj = j;
	*pk = k;
	return CO_OK;
}

static int
get3(const real * x, const real * y, const real * z,
	 int i, int j, int k, /**/ real a[3], real b[3], real c[3])
{
	vec_get(i, x, y, z, /**/ a);
	vec_get(j, x, y, z, /**/ b);
	vec_get(k, x, y, z, /**/ c);
	return CO_OK;
}

static void
zero(int n, real * a)
{
	int i;

	for (i = 0; i < n; i++)
		a[i] = 0;
}

static real
sum(int n, const real * volume)
{
	int t;
	real v;

	v = 0;
	for (t = 0; t < n; t++)
		v += volume[t];
	return v;
}

int
he_f_gompper_ini(real Kb, real C0, real Kad, real DA0D, He * he, T ** pq)
{
	T *q;
	int nv, ne, nt, nh;

	MALLOC(1, &q);

	nv = he_nv(he);
	ne = he_ne(he);
	nt = he_nt(he);
	nh = he_nh(he);

	q->nv = nv;
	q->ne = ne;
	q->nt = nt;

	q->Kb = Kb;
	q->C0 = C0;
	q->Kad = Kad;
	q->DA0D = DA0D;

	MALLOC(nh, &q->l2);
	MALLOC(nh, &q->t);

	MALLOC(nt, &q->T0);
	MALLOC(nt, &q->T1);
	MALLOC(nt, &q->T2);

	MALLOC(nv, &q->lbx);
	MALLOC(nv, &q->lby);
	MALLOC(nv, &q->lbz);
	MALLOC(nv, &q->normx);
	MALLOC(nv, &q->normy);
	MALLOC(nv, &q->normz);
	MALLOC(nv, &q->curva_mean);
	MALLOC(nv, &q->curva_gauss);

	MALLOC(nv, &q->energy);
	MALLOC(nv, &q->area);

	*pq = q;
	return CO_OK;
}

int
he_f_gompper_argv(char ***p, He * he, T ** pq)
{
	int status;
	real Kb, C0, Kad, DA0D;

	if ((status = argv_real(p, &Kb)) != CO_OK)
		return status;
	if ((status = argv_real(p, &C0)) != CO_OK)
		return status;
	if ((status = argv_real(p, &Kad)) != CO_OK)
		return status;
	if ((status = argv_real(p, &DA0D)) != CO_OK)
		return status;

	return he_f_gompper_ini(Kb, C0, Kad, DA0D, he, pq);
}

int
he_f_gompper_fin(T * q)
{
	FREE(q->T0);
	FREE(q->T1);
	FREE(q->T2);
	FREE(q->t);
	FREE(q->l2);
	FREE(q->lbx);
	FREE(q->lby);
	FREE(q->lbz);
	FREE(q->normx);
	FREE(q->normy);
	FREE(q->normz);
	FREE(q->curva_mean);
	FREE(q->curva_gauss);
	FREE(q->energy);
	FREE(q->area);
	FREE(q);
	return CO_OK;
}

int
he_f_gompper_area_ver(T * q, /**/ real ** pa)
{
	*pa = q->area;
	return CO_OK;
}

int
he_f_gompper_laplace_ver(T * q, /**/ real ** px, real ** py, real ** pz)
{
	*px = q->lbx;
	*py = q->lby;
	*pz = q->lbz;
	return CO_OK;
}

int
he_f_gompper_norm_ver(T * q, /**/ real ** px, real ** py, real ** pz)
{
	*px = q->normx;
	*py = q->normy;
	*pz = q->normz;
	return CO_OK;
}

int
he_f_gompper_curva_mean_ver(T * q, /**/ real ** pa)
{
	*pa = q->curva_mean;
	return CO_OK;
}

int
he_f_gompper_curva_gauss_ver(T * q, /**/ real ** pa)
{
	*pa = q->curva_gauss;
	return CO_OK;
}

int
he_f_gompper_energy_ver(T * q, /**/ real ** pa)
{
	*pa = q->energy;
	return CO_OK;
}

static void
compute_l2(He * he, const real * x, const real * y, const real * z,
		   /**/ real * H)
{
	int nh;
	int h, n;
	int i, j;
	real r[3];

	nh = he_nh(he);
	for (h = 0; h < nh; h++) {
		n = nxt(h);
		i = ver(h);
		j = ver(n);
		get_edg(i, j, x, y, z, /**/ r);
		H[h] = vec_dot(r, r);
	}
}

static void
compute_cot(He * he, const real * x, const real * y, const real * z,
			/**/ real * H)
{
	int nh, h, n, nn;
	int i, j, k;
	real a[3], b[3], c[3], cot;

	nh = he_nh(he);
	zero(nh, H);
	for (h = 0; h < nh; h++) {
		n = nxt(h);
		nn = nxt(n);
		i = ver(h);
		j = ver(n);
		k = ver(nn);
		get3(x, y, z, i, j, k, /**/ a, b, c);
		cot = tri_cot(b, c, a);
		H[h] += cot;
		if (!bnd(h))
			H[flp(h)] += cot;
	}
}

static void
compute_area(He * he, const real * l2, const real * t, /**/ real * V)
{
	int nv, nh, h, i;

	nv = he_nv(he);
	zero(nv, V);
	nh = he_nh(he);
	for (h = 0; h < nh; h++) {
		i = ver(h);
		V[i] += t[h] * l2[h] / 8;
	}
}

static void
compute_laplace(He * he, const real * V0, const real * t,
				const real * area, /**/ real * V1)
{
	int h, n, nv, nh, i, j;

	nv = he_nv(he);
	zero(nv, V1);
	nh = he_nh(he);
	for (h = 0; h < nh; h++) {
		n = nxt(h);
		i = ver(h);
		j = ver(n);
		V1[i] += t[h] * (V0[i] - V0[j]) / 2;
	}
	for (i = 0; i < nv; i++)
		V1[i] /= area[i];
}

static int
compute_norm(T * q, He * he,
			 const real * x, const real * y, const real * z, /**/
			 real * normx, real * normy, real * normz)
{
	enum { X, Y, Z };
	int t, nt;
	int i, j, k, nv;
	real a[3], b[3], c[3], u[3], u0[3];
	int *T0, *T1, *T2;
	real theta_a, theta_b, theta_c;

	nt = he_nt(he);
	nv = he_nv(he);
	T0 = q->T0;
	T1 = q->T1;
	T2 = q->T2;

	zero(nv, normx);
	zero(nv, normy);
	zero(nv, normz);
	for (t = 0; t < nt; t++) {
		i = T0[t];
		j = T1[t];
		k = T2[t];
		get3(x, y, z, i, j, k, a, b, c);
		theta_a = tri_angle(c, a, b);
		theta_b = tri_angle(a, b, c);
		theta_c = tri_angle(b, c, a);
		tri_normal(a, b, c, u);
		vec_scalar_append(u, theta_a, i, normx, normy, normz);
		vec_scalar_append(u, theta_b, j, normx, normy, normz);
		vec_scalar_append(u, theta_c, k, normx, normy, normz);
	}

	for (i = 0; i < nv; i++) {
		vec_get(i, normx, normy, normz, /**/ u);
		vec_norm(u, /**/ u0);
		vec_set(u0, i, /**/ normx, normy, normz);
		//vec_negative(u0, u); /*This reverses the sign of norm to be inwards*/
		//vec_set(u, i, /**/ normx, normy, normz);
	}
	return CO_OK;
}

static int
compute_curva_mean(He * he,
				   real * lbx, real * lby, real * lbz,
				   real * normx, real * normy, real * normz,
				   /**/ real * curva_mean)
{
	enum { X, Y, Z };
	int i, nv;
	real u[3], v[3];

	nv = he_nv(he);

	for (i = 0; i < nv; i++) {
		vec_get(i, lbx, lby, lbz, u);
		vec_get(i, normx, normy, normz, v);
		curva_mean[i] = vec_dot(u, v) / 2;
	}

	return CO_OK;

}

static real
compute_energy_local(T * q, const real * curva_mean, const real * area,
					 /**/ real * energy)
{

	real Kb, C0, H0;
	int i, nv;
	real energy_tot;

	Kb = q->Kb;
	C0 = q->C0;
	nv = q->nv;

	H0 = C0 / 2.0;

	energy_tot = 0;

	for (i = 0; i < nv; i++) {
		energy[i] =
			2 * Kb * (curva_mean[i] - H0) * (curva_mean[i] - H0) * area[i];
		energy_tot += energy[i];
	}

	return energy_tot;
}

static real
compute_curva_mean_integral(T * q, const real * curva_mean,
							const real * area)
{

	int i, nv;
	real cm_integral;

	nv = q->nv;

	cm_integral = 0;
	for (i = 0; i < nv; i++) {
		cm_integral += curva_mean[i] * area[i];
	}
	return cm_integral;
}

static real
compute_energy_nonlocal(T * q, const real * curva_mean, const real * area)
{

	real Kad, DA0D;
	int nv;
	real cm_integral, area_tot, energy_tot;

	Kad = q->Kad;
	DA0D = q->DA0D;
	nv = q->nv;

	cm_integral = compute_curva_mean_integral(q, curva_mean, area);
	area_tot = sum(nv, area);
	energy_tot =
		4 * pi * Kad / area_tot * (cm_integral - DA0D / 2) * (cm_integral -
															  DA0D / 2);

	return energy_tot;
}

real
he_f_gompper_energy(T * q, He * he,
					const real * x, const real * y, const real * z)
{
	int nv, nt;
	int i, j, k, l;
	int *T0, *T1, *T2;

	real *l2, *t;
	real *lbx, *lby, *lbz;
	real *normx, *normy, *normz;
	real *curva_mean;
	real *energy, *area;
	real eng_bend, eng_ad;

	T0 = q->T0;
	T1 = q->T1;
	T2 = q->T2;
	l2 = q->l2;
	t = q->t;
	lbx = q->lbx;
	lby = q->lby;
	lbz = q->lbz;
	normx = q->normx;
	normy = q->normy;
	normz = q->normz;
	curva_mean = q->curva_mean;
	area = q->area;
	energy = q->energy;

	nv = q->nv;
	nt = he_nt(he);

	for (l = 0; l < nt; l++) {
		get_ijk(l, he, /**/ &i, &j, &k);
		T0[l] = i;
		T1[l] = j;
		T2[l] = k;
	}

	if (he_nv(he) != nv)
		ERR(CO_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

	compute_l2(he, x, y, z, /**/ l2);
	compute_cot(he, x, y, z, /**/ t);
	compute_area(he, l2, t, /**/ area);
	compute_laplace(he, x, t, area, /**/ lbx);
	compute_laplace(he, y, t, area, /**/ lby);
	compute_laplace(he, z, t, area, /**/ lbz);
	compute_norm(q, he, x, y, z, normx, normy, normz);
	compute_curva_mean(he, lbx, lby, lbz, normx, normy, normz,
					   /**/ curva_mean);

	eng_bend = compute_energy_local(q, curva_mean, area, /**/ energy);
	eng_ad = compute_energy_nonlocal(q, curva_mean, area);

	q->eng_bend = eng_bend;
	q->eng_ad = eng_ad;

	return eng_bend + eng_ad;
}

static void
compute_force_t(T * q, He * he,
				const real * x, const real * y, const real * z,
				const real * t,
				const real * lbx, const real * lby, const real * lbz,
				/**/ real * fx, real * fy, real * fz)
{
	real Kb;
	int nh;
	int h, n;
	int i, j;
	real r[3], ll[3], df[3];
	real t0, l2;

	Kb = q->Kb;

	nh = he_nh(he);

	for (h = 0; h < nh; h++) {
		n = nxt(h);
		i = ver(h);
		j = ver(n);
		t0 = t[h];
		get_edg(i, j, x, y, z, /**/ r);
		vec_get(i, lbx, lby, lbz, ll);
		l2 = vec_dot(ll, ll);
		vec_linear_combination(Kb * t0 / 2, ll, -Kb * t0 * l2 / 8, r, df);
		vec_append(df, i, /**/ fx, fy, fz);
		vec_substr(df, j, /**/ fx, fy, fz);
	}

}

static void
compute_force_dt(T * q, He * he,
				 const real * x, const real * y, const real * z,
				 const real * lbx, const real * lby, const real * lbz,
				 /**/ real * fx, real * fy, real * fz)
{
	real Kb;
	int nh;
	int h, n, nn;
	int i, j, k;
	real r[3], a[3], b[3], c[3], da[3], db[3], dc[3];
	real li[3], lk[3];
	real dl, dd, r2, C;

	Kb = q->Kb;
	nh = he_nh(he);

	for (h = 0; h < nh; h++) {
		n = nxt(h);
		nn = nxt(n);
		i = ver(h);
		j = ver(n);
		k = ver(nn);
		get3(x, y, z, i, j, k, a, b, c);
		dtri_cot(a, b, c, /**/ da, db, dc);
		get_edg(i, k, x, y, z, /**/ r);
		vec_get(k, lbx, lby, lbz, /**/ lk);
		vec_get(i, lbx, lby, lbz, /**/ li);
		r2 = vec_dot(r, r);
		dl = vec_dot(lk, lk) + vec_dot(li, li);
		dd = vec_dot(li, r) - vec_dot(lk, r);
		C = Kb * dd / 2 - Kb * r2 * dl / 16;
		vec_scalar_append(da, C, i, /**/ fx, fy, fz);
		vec_scalar_append(db, C, j, /**/ fx, fy, fz);
		vec_scalar_append(dc, C, k, /**/ fx, fy, fz);
	}

}

int
he_f_gompper_force(T * q, He * he,
				   const real * x, const real * y, const real * z, /**/
				   real * fx, real * fy, real * fz)
{
	int nv, nt;
	int i, j, k, l;
	int *T0, *T1, *T2;

	real *l2, *t;
	real *lbx, *lby, *lbz;
	real *area;


	T0 = q->T0;
	T1 = q->T1;
	T2 = q->T2;
	area = q->area;
	l2 = q->l2;
	t = q->t;
	lbx = q->lbx;
	lby = q->lby;
	lbz = q->lbz;

	nv = q->nv;

	nt = he_nt(he);
	for (l = 0; l < nt; l++) {
		get_ijk(l, he, /**/ &i, &j, &k);
		T0[l] = i;
		T1[l] = j;
		T2[l] = k;
	}

	if (he_nv(he) != nv)
		ERR(CO_INDEX, "he_nv(he)=%d != nv = %d", he_nv(he), nv);

	compute_l2(he, x, y, z, /**/ l2);
	compute_cot(he, x, y, z, /**/ t);
	compute_area(he, l2, t, /**/ area);
	compute_laplace(he, x, t, area, /**/ lbx);
	compute_laplace(he, y, t, area, /**/ lby);
	compute_laplace(he, z, t, area, /**/ lbz);

	compute_force_t(q, he, x, y, z, t, lbx, lby, lbz, /**/ fx, fy, fz);
	compute_force_dt(q, he, x, y, z, lbx, lby, lbz, /**/ fx, fy, fz);

	return CO_OK;
}

real
he_f_gompper_energy_ad(T * q)
{
	return q->eng_ad;
}

real
he_f_gompper_energy_bend(T * q)
{
	return q->eng_bend;
}
