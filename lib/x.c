#include <stdio.h>
#include <stdarg.h>

#include "real.h"

#include "co/area.h"
#include "co/bending.h"
#include "co/equiangulate.h"
#include "co/err.h"
#include "co/f/area.h"
#include "co/f/area_voronoi.h"
#include "co/f/edg_sq.h"
#include "co/f/garea.h"
#include "co/f/garea_voronoi.h"
#include "co/f/gompper.h"
#include "co/f/gompper_kroll.h"
#include "co/f/harmonic.h"
#include "co/f/harmonic_ref.h"
#include "co/f/kantor.h"
#include "co/f/volume.h"
#include "co/f/volume_normal.h"
#include "co/he.h"
#include "co/memory.h"
#include "co/off.h"
#include "co/read.h"
#include "co/restore.h"
#include "co/vec.h"
#include "co/volume.h"
#include "co/filter.h"

#include "co/x.h"

int NV, NE, NT, NH;
int *T0, *T1, *T2;
int *D0, *D1, *D2, *D3;
real *XX, *YY, *ZZ;
real *VX, *VY, *VZ;
real *RR, *TH;

static He *he;
static HeFArea *f_area;
static HeFAreaVoronoi *f_area_voronoi;
static HeFGareaVoronoi *f_garea_voronoi;
static HeFGarea *f_garea;
static HeFVolume *f_volume;
static HeFVolumeNormal *f_volume_normal;
static HeFHarmonic *f_harmonic;
static HeFEdgSq *f_edg_sq;
static HeFHarmonicRef *f_harmonic_ref;
static HeFKantor *f_kantor;
static HeFGompper *f_gompper;
static HeFGompperKroll *f_gompper_kroll;
static Bending *f_bending;
static Restore *f_restore;
static Filter *f_filter;
static HeOff *off;

int
nxt(int h)
{
  return he_nxt(he, h);
}

int
flp(int h)
{
  return he_flp(he, h);
}

int
ver(int h)
{
  return he_ver(he, h);
}

int
edg(int h)
{
  return he_edg(he, h);
}

int
tri(int h)
{
  return he_tri(he, h);
}

int
hdg_ver(int v)
{
  return he_hdg_ver(he, v);
}

int
hdg_edg(int e)
{
  return he_hdg_edg(he, e);
}

int
hdg_tri(int t)
{
  return he_hdg_tri(he, t);
}

int
bnd(int h)
{
  return he_bnd(he, h);
}

real
area()
{
  return he_area(he, XX, YY, ZZ);
}

real
volume()
{
  return he_volume_tri(he, XX, YY, ZZ);
}

int
off_write(const real * x, const real * y, const real * z, const char *path)
{
  return off_he_xyz_write(he, x, y, z, path);
}

int
RZERO(int n, real ** pq)
{                               /* alloc and make zero */
  int i;
  real *q;

  MALLOC(n, &q);
  for (i = 0; i < n; i++)
    q[i] = 0;
  *pq = q;
  return CO_OK;
}

int
ini(const char *path)
{
  int i, j, k, l, h, e, n, nn, nnf;
  int *tri, *tri0;
  real *xyz, *xyz0;
  real r[3];
  HeRead *read;

  off_ini(path, &off);

  NV = off_nv(off);
  NT = off_nt(off);

  off_tri(off, &tri0);
  off_ver(off, &xyz0);
  he_read_tri_ini(NV, NT, tri0, &read);

  he_ini(read, &he);
  NV = he_nv(he);
  NT = he_nt(he);
  NE = he_ne(he);
  NH = he_nh(he);

  MALLOC(NV, &XX);
  MALLOC(NV, &YY);
  MALLOC(NV, &ZZ);
  MALLOC(NV, &VX);
  MALLOC(NV, &VY);
  MALLOC(NV, &VZ);
  MALLOC(NV, &RR);
  MALLOC(NV, &TH);
  MALLOC(NT, &T0);
  MALLOC(NT, &T1);
  MALLOC(NT, &T2);
  MALLOC(NE, &D0);
  MALLOC(NE, &D1);
  MALLOC(NE, &D2);
  MALLOC(NE, &D3);

  for (xyz = xyz0, i = 0; i < NV; i++) {
    XX[i] = *xyz++;
    YY[i] = *xyz++;
    ZZ[i] = *xyz++;
  }
  for (i = 0; i < NV; i++) {
    vec_get(i, XX, YY, ZZ, r);
    RR[i] = vec_cylindrical_r(r);
    TH[i] = vec_spherical_theta(r);
  }

  for (tri = tri0, i = 0; i < NT; i++) {
    T0[i] = *tri++;
    T1[i] = *tri++;
    T2[i] = *tri++;
  }

  for (e = 0; e < NE; e++) {    /* i[jk]l */
    h = hdg_edg(e);
    n = nxt(h);
    nn = nxt(nxt(h));
    j = ver(h);
    k = ver(n);
    i = ver(nn);
    if (!bnd(h)) {
      nnf = nxt(nxt(flp(h)));
      l = ver(nnf);
    } else
      l = -1;
    D0[e] = i;
    D1[e] = j;
    D2[e] = k;
    D3[e] = l;
  }

  he_read_fin(read);

  return CO_OK;
}

int
fin()
{
  FREE(XX);
  FREE(YY);
  FREE(ZZ);
  FREE(VX);
  FREE(VY);
  FREE(VZ);
  FREE(RR);
  FREE(TH);
  FREE(T0);
  FREE(T1);
  FREE(T2);
  FREE(D0);
  FREE(D1);
  FREE(D2);
  FREE(D3);

  off_fin(off);
  he_fin(he);
  return CO_OK;
}

int
x_he(He ** phe)
{
  *phe = he;
  return CO_OK;
}

int
equiangulate(int *cnt)
{
  return he_equiangulate(he, XX, YY, ZZ, cnt);
}

int
f_area_ini(real a0, real K)
{
  he_f_area_ini(a0, K, he, /**/ &f_area);
  return CO_OK;
}

int
f_area_fin()
{
  he_f_area_fin(f_area);
  return CO_OK;
}

real
f_area_energy(const real * x, const real * y, const real * z)
{
  return he_f_area_energy(f_area, he, x, y, z);
}

int
f_area_force(const real * x, const real * y, const real * z,
             /**/ real * fx, real * fy, real * fz)
{
  return he_f_area_force(f_area, he, x, y, z, /**/ fx, fy, fz);
}

int
f_area_voronoi_ini(real a0, real K)
{
  he_f_area_voronoi_ini(a0, K, he, /**/ &f_area_voronoi);
  return CO_OK;
}

int
f_area_voronoi_fin()
{
  he_f_area_voronoi_fin(f_area_voronoi);
  return CO_OK;
}

real
f_area_voronoi_energy(const real * x, const real * y, const real * z)
{
  return he_f_area_voronoi_energy(f_area_voronoi, he, x, y, z);
}

int
f_area_voronoi_force(const real * x, const real * y, const real * z,
                     /**/ real * fx, real * fy, real * fz)
{
  return he_f_area_voronoi_force(f_area_voronoi, he, x, y, z, /**/ fx,
                                 fy, fz);
}

int
f_volume_normal_ini(real a0, real K)
{
  he_f_volume_normal_ini(a0, K, he, /**/ &f_volume_normal);
  return CO_OK;
}

int
f_volume_normal_fin()
{
  he_f_volume_normal_fin(f_volume_normal);
  return CO_OK;
}

real
f_volume_normal_energy(const real * x, const real * y, const real * z)
{
  return he_f_volume_normal_energy(f_volume_normal, he, x, y, z);
}

int
f_volume_normal_force(const real * x, const real * y, const real * z,
                      /**/ real * fx, real * fy, real * fz)
{
  return he_f_volume_normal_force(f_volume_normal, he, x, y, z, /**/ fx,
                                  fy, fz);
}

int
f_garea_voronoi_ini(real a0, real K)
{
  he_f_garea_voronoi_ini(a0, K, he, /**/ &f_garea_voronoi);
  return CO_OK;
}

int
f_garea_voronoi_fin()
{
  he_f_garea_voronoi_fin(f_garea_voronoi);
  return CO_OK;
}

real
f_garea_voronoi_energy(const real * x, const real * y, const real * z)
{
  return he_f_garea_voronoi_energy(f_garea_voronoi, he, x, y, z);
}

int
f_garea_voronoi_force(const real * x, const real * y, const real * z,
                      /**/ real * fx, real * fy, real * fz)
{
  return he_f_garea_voronoi_force(f_garea_voronoi, he, x, y, z, /**/ fx,
                                  fy, fz);
}

int
f_garea_ini(real a0, real K)
{
  he_f_garea_ini(a0, K, he, /**/ &f_garea);
  return CO_OK;
}

int
f_garea_fin()
{
  he_f_garea_fin(f_garea);
  return CO_OK;
}

real
f_garea_energy(const real * x, const real * y, const real * z)
{
  return he_f_garea_energy(f_garea, he, x, y, z);
}

int
f_garea_force(const real * x, const real * y, const real * z,
              /**/ real * fx, real * fy, real * fz)
{
  return he_f_garea_force(f_garea, he, x, y, z, /**/ fx, fy, fz);
}

int
f_volume_ini(real a0, real K)
{
  he_f_volume_ini(a0, K, he, /**/ &f_volume);
  return CO_OK;
}

int
f_volume_set_v(real v)
{
  he_f_volume_set_v(f_volume, v);
  return CO_OK;
}

int
f_volume_fin()
{
  he_f_volume_fin(f_volume);
  return CO_OK;
}

real
f_volume_energy(const real * x, const real * y, const real * z)
{
  return he_f_volume_energy(f_volume, he, x, y, z);
}

int
f_volume_force(const real * x, const real * y, const real * z,
               /**/ real * fx, real * fy, real * fz)
{
  return he_f_volume_force(f_volume, he, x, y, z, /**/ fx, fy, fz);
}

int
f_harmonic_ini(real a0, real K)
{
  he_f_harmonic_ini(a0, K, he, /**/ &f_harmonic);
  return CO_OK;
}

int
f_harmonic_fin()
{
  he_f_harmonic_fin(f_harmonic);
  return CO_OK;
}

real
f_harmonic_energy(const real * x, const real * y, const real * z)
{
  return he_f_harmonic_energy(f_harmonic, he, x, y, z);
}

int
f_harmonic_force(const real * x, const real * y, const real * z,
                 /**/ real * fx, real * fy, real * fz)
{
  return he_f_harmonic_force(f_harmonic, he, x, y, z, /**/ fx, fy, fz);
}

int
f_edg_sq_ini(real K)
{
  he_f_edg_sq_ini(K, he, /**/ &f_edg_sq);
  return CO_OK;
}

int
f_edg_sq_fin()
{
  he_f_edg_sq_fin(f_edg_sq);
  return CO_OK;
}

real
f_edg_sq_energy(const real * x, const real * y, const real * z)
{
  return he_f_edg_sq_energy(f_edg_sq, he, x, y, z);
}

int
f_edg_sq_force(const real * x, const real * y, const real * z,
               /**/ real * fx, real * fy, real * fz)
{
  return he_f_edg_sq_force(f_edg_sq, he, x, y, z, /**/ fx, fy, fz);
}

int
f_harmonic_ref_ini(real K, const real * x, const real * y, const real * z)
{
  he_f_harmonic_ref_ini(K, x, y, z, he, /**/ &f_harmonic_ref);
  return CO_OK;
}

int
f_harmonic_ref_fin()
{
  he_f_harmonic_ref_fin(f_harmonic_ref);
  return CO_OK;
}

real
f_harmonic_ref_energy(const real * x, const real * y, const real * z)
{
  return he_f_harmonic_ref_energy(f_harmonic_ref, he, x, y, z);
}

int
f_harmonic_ref_force(const real * x, const real * y, const real * z,
                     /**/ real * fx, real * fy, real * fz)
{
  return he_f_harmonic_ref_force(f_harmonic_ref, he, x, y, z, /**/ fx,
                                 fy, fz);
}

int
f_kantor_ini(real K, real theta0)
{
  he_f_kantor_ini(K, theta0, he, /**/ &f_kantor);
  return CO_OK;
}

int
f_kantor_fin()
{
  he_f_kantor_fin(f_kantor);
  return CO_OK;
}

real
f_kantor_energy(const real * x, const real * y, const real * z)
{
  return he_f_kantor_energy(f_kantor, he, x, y, z);
}

int
f_kantor_force(const real * x, const real * y, const real * z,
               /**/ real * fx, real * fy, real * fz)
{
  return he_f_kantor_force(f_kantor, he, x, y, z, /**/ fx, fy, fz);
}

int
f_gompper_ini(real Kb, real C0, real Kad, real DA0D)
{
  he_f_gompper_ini(Kb, C0, Kad, DA0D, he, /**/ &f_gompper);
  return CO_OK;
}

int
f_gompper_fin()
{
  he_f_gompper_fin(f_gompper);
  return CO_OK;
}

real
f_gompper_energy(const real * x, const real * y, const real * z)
{
  return he_f_gompper_energy(f_gompper, he, x, y, z);
}

int
f_gompper_force(const real * x, const real * y, const real * z,
                /**/ real * fx, real * fy, real * fz)
{
  return he_f_gompper_force(f_gompper, he, x, y, z, /**/ fx, fy, fz);
}

int
f_gompper_area_ver(real ** p)
{
  return he_f_gompper_area_ver(f_gompper, p);
}

int
f_gompper_laplace_ver(real ** px, real ** py, real ** pz)
{
  return he_f_gompper_laplace_ver(f_gompper, px, py, pz);
}

int
f_gompper_norm_ver(real ** px, real ** py, real ** pz)
{
  return he_f_gompper_norm_ver(f_gompper, px, py, pz);
}

int
f_gompper_curva_mean_ver(real ** p)
{
  return he_f_gompper_curva_mean_ver(f_gompper, p);
}

int
f_gompper_curva_gauss_ver(real ** p)
{
  return he_f_gompper_curva_gauss_ver(f_gompper, p);
}

int
f_gompper_energy_ver(real ** p)
{
  return he_f_gompper_energy_ver(f_gompper, p);
}

int
f_gompper_kroll_ini(real Kb, real C0, real Kad, real DA0D)
{
  he_f_gompper_kroll_ini(Kb, C0, Kad, DA0D, he, /**/ &f_gompper_kroll);
  return CO_OK;
}

int
f_gompper_kroll_fin()
{
  he_f_gompper_kroll_fin(f_gompper_kroll);
  return CO_OK;
}

real
f_gompper_kroll_energy(const real * x, const real * y, const real * z)
{
  return he_f_gompper_kroll_energy(f_gompper_kroll, he, x, y, z);
}

int
f_gompper_kroll_force(const real * x, const real * y, const real * z,
                      /**/ real * fx, real * fy, real * fz)
{
  return he_f_gompper_kroll_force(f_gompper_kroll, he, x, y, z, /**/ fx,
                                  fy, fz);
}

int
f_gompper_kroll_area_ver(real ** p)
{
  return he_f_gompper_kroll_area_ver(f_gompper_kroll, p);
}

int
f_gompper_kroll_laplace_ver(real ** px, real ** py, real ** pz)
{
  return he_f_gompper_kroll_laplace_ver(f_gompper_kroll, px, py, pz);
}

int
f_gompper_kroll_norm_ver(real ** px, real ** py, real ** pz)
{
  return he_f_gompper_kroll_norm_ver(f_gompper_kroll, px, py, pz);
}

int
f_gompper_kroll_curva_mean_ver(real ** p)
{
  return he_f_gompper_kroll_curva_mean_ver(f_gompper_kroll, p);
}

int
f_gompper_kroll_curva_gauss_ver(real ** p)
{
  return he_f_gompper_kroll_curva_gauss_ver(f_gompper_kroll, p);
}

int
f_gompper_kroll_energy_ver(real ** p)
{
  return he_f_gompper_kroll_energy_ver(f_gompper_kroll, p);
}

int
f_bending_ini(const char *name, BendingParam param)
{
  bending_ini(name, param, he, /**/ &f_bending);
  return CO_OK;
}

int
f_bending_fin()
{
  bending_fin(f_bending);
  return CO_OK;
}

real
f_bending_energy(const real * x, const real * y, const real * z)
{
  return bending_energy(f_bending, he, x, y, z);
}

int
f_bending_force(const real * x, const real * y, const real * z,
                /**/ real * fx, real * fy, real * fz)
{
  return bending_force(f_bending, he, x, y, z, /**/ fx, fy, fz);
}

int
f_bending_energy_ver(real ** p)
{
  return bending_energy_ver(f_bending, p);
}

real
f_bending_energy_bend(void)
{
  return bending_energy_bend(f_bending);
}

real
f_bending_energy_ad(void)
{
  return bending_energy_ad(f_bending);
}

int
x_restore_ini(real volume)
{
  return restore_ini(volume, he, &f_restore);
}

int
x_restore_fin(void)
{
  return restore_fin(f_restore);
}

int
x_restore_volume(real * x, real * y, real * z)
{
  return restore_volume(f_restore, he, x, y, z);
}

int
x_filter_ini(void)
{
  return filter_ini(he, &f_filter);
}

int
x_filter_fin(void)
{
  return filter_fin(f_filter);
}

int
x_filter_apply(const real * x, const real * y, const real * z,  /*io */
               real * a)
{
  return filter_apply(f_filter, he, x, y, z, a);
}

int
x_get_he(He ** phe)
{
  *phe = he;
  return CO_OK;
}
