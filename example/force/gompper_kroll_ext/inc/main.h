static void write(real *fx, real *fy, real *fz,
                  real *lx, real *ly, real *lz,
                  real *A) {

  real *fm, *lm;
  RZERO(NV, &fm); RZERO(NV, &lm);
  
  vabs(NV, fx, fy, fz, /**/ fm);
  vabs(NV, lx, ly, lz, /**/ lm);
  
  printf("#1 azimuth angle; 2 axis dist; 3 zz; 4 |F|; 5 area; 6; |laplace-beltrami|\n");
  real *queue[] = {TH, RR, ZZ, fm, A, lm, NULL};
  punto_fwrite(NV, queue, stdout);
  
  FREE(lm);  FREE(fm);
  
}

static void gompper_kroll_ext() {

  real *cot, *lensq, *area, area_tot;
  real *lbx, *lby, *lbz;
  real *nx, *ny, *nz;
  real *cm, cm_intga;
  real *fx, *fy, *fz;
  
  RZERO(NH, &cot); RZERO(NH, &lensq); RZERO(NV, &area);
  RZERO(NV, &lbx); RZERO(NV, &lby);   RZERO(NV, &lbz);
  RZERO(NV, &nx);  RZERO(NV, &ny);    RZERO(NV, &nz);
  RZERO(NV, &cm);
  RZERO(NV, &fx);  RZERO(NV, &fy);    RZERO(NV, &fz);

  mesh_cot(/*o*/ cot);
  mesh_lensq(/*o*/ lensq);
  area_tot = mesh_area_voronoi(cot, lensq, /*io*/ area);
  
  mesh_laplace(XX, cot, area, /*io*/ lbx);
  mesh_laplace(YY, cot, area, /*io*/ lby);
  mesh_laplace(ZZ, cot, area, /*io*/ lbz);

  mesh_norm(/*io*/ nx, ny, nz);

  cm_intga = mesh_curvature_mean(lbx, lby,lbz, nx, ny,nz, area, /*io*/ cm);
  
  mesh_force (cot, area, area_tot, lbx, lby, lbz, cm, cm_intga, /*io*/ fx, fy, fz);

  write(/*i*/ fx, fy, fz, lbx, lby, lbz, area);
  
  FREE(cot); FREE(lensq); FREE(area);
  FREE(lbx); FREE(lby);   FREE(lbz);
  FREE(nx);  FREE(ny);    FREE(nz);
  FREE(cm);  
  FREE(fx);  FREE(fy);    FREE(fz);
  
}

int main() {
    ini("/dev/stdin");
    gompper_kroll_ext();
    fin();
    return HE_OK;
}
