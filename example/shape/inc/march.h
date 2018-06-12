static void march_integrate_velocity_verlet() {
  
  real *cot, *lensq;
  real *area_voro, area_voro_tot;
  real *area_tri, *area_tri0;
  real area_tri_tot, area_tri_tot0;
  real vol_tot, vol_tot0;  
  real *lbx, *lby, *lbz;
  real *nx, *ny, *nz;
  real *cm, cm_intga;
  real *fx, *fy, *fz;

  int i;
  int iter, iter_num;
  real mass, dt;
  
  RZERO(NH, &cot);  RZERO(NH, &lensq);
  RZERO(NV, &area_voro);
  RZERO(NV, &area_tri); RZERO(NV, &area_tri0);
  RZERO(NV, &lbx);  RZERO(NV, &lby); RZERO(NV, &lbz);
  RZERO(NV, &nx);   RZERO(NV, &ny);  RZERO(NV, &nz);
  RZERO(NV, &cm);
  RZERO(NV, &fx);   RZERO(NV, &fy);  RZERO(NV, &fz);

  /*initialization*/
  mesh_cot(/*o*/ cot);
  mesh_lensq(/*o*/ lensq);
  area_voro_tot = mesh_area_voronoi(cot, lensq, /*io*/ area_voro);
  area_tri_tot0 = mesh_area_total(/*o*/ area_tri0);
  
  vol_tot0  = mesh_volume_total();
  
  mesh_laplace(XX, cot, area_voro, /*io*/ lbx);
  mesh_laplace(YY, cot, area_voro, /*io*/ lby);
  mesh_laplace(ZZ, cot, area_voro, /*io*/ lbz);

  mesh_norm(/*io*/ nx, ny, nz);

  cm_intga = mesh_curvature_mean(lbx, lby, lbz, nx, ny, nz, area_voro, /*io*/ cm);
  
  mesh_force(cot, area_voro, area_voro_tot, lbx, lby, lbz, cm, cm_intga, /*io*/ fx, fy, fz);
  
  /*mesh_force_area(area, area0, area_tot, area_tot0, fx, fy, fz);
    mesh_force_volume(vol_tot, vol_tot0, fx, fy, fz);*/
  

  iter_num = 1;
  iter = 0;
  mass = 1.0;
  dt   = 1.0e-8;
  /*time loop*/
  
  data_reset(VX, NV);
  data_reset(VY, NV);
  data_reset(VZ, NV);

  write(/*i*/ XX, YY, ZZ, VX, VY, VZ, fx, fy, fz);
  printf("\n\n");

  while ( iter < iter_num ) {
    
    for ( i = 0; i < NV; i ++ ) {
      
      VX[i] += fx[i] * dt / mass / 2.0;
      VY[i] += fy[i] * dt / mass / 2.0;
      VZ[i] += fz[i] * dt / mass / 2.0;

      XX[i] += VX[i] * dt;
      YY[i] += VY[i] * dt;
      ZZ[i] += VZ[i] * dt;
    }

    mesh_cot(/*o*/ cot);
    mesh_lensq(/*o*/ lensq);
    area_voro_tot = mesh_area_voronoi(cot, lensq, /*io*/ area_voro);
    area_tri_tot  = mesh_area_total(/*o*/ area_tri);

    vol_tot  = mesh_volume_total();
    
    mesh_laplace(XX, cot, area_voro, /*io*/ lbx);
    mesh_laplace(YY, cot, area_voro, /*io*/ lby);
    mesh_laplace(ZZ, cot, area_voro, /*io*/ lbz);
    
    mesh_norm(/*io*/ nx, ny, nz);
    
    cm_intga = mesh_curvature_mean(lbx, lby, lbz, nx, ny, nz, area_voro, /*io*/ cm);
  
    mesh_force(cot, area_voro, area_voro_tot, lbx, lby, lbz, cm, cm_intga, /*io*/ fx, fy, fz);
  
    mesh_force_area(area_tri, area_tri0, area_tri_tot, area_tri_tot0, fx, fy, fz);
    /*  mesh_force_volume(vol_tot, vol_tot0, fx, fy, fz);*/
    
    for ( i = 0; i < NV; i ++ ) {
      
      VX[i] += fx[i] * dt / mass / 2.0;
      VY[i] += fy[i] * dt / mass / 2.0;
      VZ[i] += fz[i] * dt / mass / 2.0;
      
    }

    write(/*i*/ XX, YY, ZZ, VX, VY, VZ, fx, fy, fz);
    printf("\n\n");

    iter ++;
    
  }
  
  
  FREE(cot); FREE(lensq);
  FREE(area_voro);
  FREE(area_tri0);  FREE(area_tri);
  FREE(lbx); FREE(lby);   FREE(lbz);
  FREE(nx);  FREE(ny);    FREE(nz);
  FREE(cm);  
  FREE(fx);  FREE(fy);    FREE(fz);
}
