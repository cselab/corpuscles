static void write(real *fx, real *fy, real *fz,
                  real *lx, real *ly, real *lz,
                  real *A,
		  real *cm_dx, real *cm_dy, real *cm_dz) {

  real *fm, *lm, *cmd;
  RZERO(NV, &fm); RZERO(NV, &lm); RZERO(NV, &cmd);
  
  vabs(NV, fx, fy, fz, /**/ fm);
  vabs(NV, lx, ly, lz, /**/ lm);
  vabs(NV, cm_dx, cm_dy, cm_dz, /**/ cmd);
  
  printf("#1 azimuth angle; 2 axis dist; 3 zz; 4 |F|; 5 area; 6 |laplace-beltrami|; 7 |cm_derivative|\n");
  real *queue[] = {TH, RR, ZZ, fm, A, lm, cmd, NULL};
  punto_fwrite(NV, queue, stdout);
  
  FREE(lm);  FREE(fm);  FREE(cmd);
  
}

static void gompper_kroll_ext() {

  real *cot, *lensq;
  real *area, area_tot;
  real *area0, area_tot0;
  real vol_tot, vol_tot0;  
  real *lbx, *lby, *lbz;
  real *nx, *ny, *nz;
  real *cm, cm_intga;
  real *cm_dx, *cm_dy, *cm_dz;
  real *fx, *fy, *fz;
  
  RZERO(NH, &cot); RZERO(NH, &lensq);
  RZERO(NV, &area);RZERO(NV, &area0);
  RZERO(NV, &lbx); RZERO(NV, &lby);   RZERO(NV, &lbz);
  RZERO(NV, &nx);  RZERO(NV, &ny);    RZERO(NV, &nz);
  RZERO(NV, &cm);
  RZERO(NV, &cm_dx); RZERO(NV, &cm_dy); RZERO(NV, &cm_dz);
  RZERO(NV, &fx);    RZERO(NV, &fy);    RZERO(NV, &fz);

  mesh_cot(/*o*/ cot);
  mesh_lensq(/*o*/ lensq);
  area_tot  = mesh_area_voronoi(cot, lensq, /*io*/ area);
  area_tot0 = mesh_area_total();
  data_copy(area, area0, NV);

  vol_tot  = mesh_volume_total();
  vol_tot0 = vol_tot;
  
  mesh_laplace(XX, cot, area, /*io*/ lbx);
  mesh_laplace(YY, cot, area, /*io*/ lby);
  mesh_laplace(ZZ, cot, area, /*io*/ lbz);

  mesh_norm(/*io*/ nx, ny, nz);

  cm_intga = mesh_curvature_mean(lbx, lby,lbz, nx, ny,nz, area, /*io*/ cm);
  
  mesh_force(cot, area, area_tot, lbx, lby, lbz, cm, cm_intga,
	     /*io*/ fx, fy, fz, cm_dx, cm_dy, cm_dz);
  
  mesh_force_area(area, area0, area_tot, area_tot0, fx, fy, fz);
  mesh_force_volume(vol_tot, vol_tot0, fx, fy, fz);
  
  write(/*i*/ fx, fy, fz, lbx, lby, lbz, area, cm_dx, cm_dy, cm_dz);
  
  FREE(cot); FREE(lensq); FREE(area);
  FREE(lbx); FREE(lby);   FREE(lbz);
  FREE(nx);  FREE(ny);    FREE(nz);
  FREE(cm);
  FREE(cm_dx); FREE(cm_dy); FREE(cm_dz);
  FREE(fx);  FREE(fy);    FREE(fz);
  
}

int main() {
    ini("/dev/stdin");
    gompper_kroll_ext();
    fin();
    return HE_OK;
}
