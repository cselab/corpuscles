static void mesh_cot(/*o*/ real *cot) {
  /*traverse each halfedge and calcuate cotanget
    for the angle opposite to the halfedge.
    Sum the cotanget for the halfedge. */
  
  int h, n, nn;
  int i, j, k;
  real a[3], b[3], c[3], cot0;
  
  for (h = 0; h < NH; h++) {
    
    n = nxt(h); nn = nxt(n); 
    i = ver(h); j = ver(n); k = ver(nn);
    get3(i, j, k, /**/ a, b, c);
    cot0 = tri_cot(b, c, a);
    cot[h] += cot0;
    
    if (!bnd(h)) cot[flp(h)] += cot0;
    
  }
  
}

static void mesh_lensq(/*o*/ real *lensq) {
  /*traverse each halfedge and calculate its length squared*/
  
  int h, n;
  int i, j;
  real r[3];
  
  for (h = 0; h < NH; h++) {
    
    n = nxt(h);
    i = ver(h); j = ver(n);
    get_edg(i, j, /**/ r);
    lensq[h] = vec_dot(r, r);
    
  }
  
}

static real mesh_area_voronoi(real *cot, real *lensq,
			      /*io*/ real *area) {
  /*traverse each halfedge and calcuate voronoi area for each vertex.
    Sum the area for the vertex due to each voronoi area contribution 
    from neigboring triangles.
    In the end, return the area for each vertex and the total area.
  */
  
  int h, i;
  real area0, area_tot;

  area_tot = 0.0;
  
  for (h = 0; h < NH; h++) {
    i = ver(h);
    area0 = cot[h]*lensq[h]/8;
    area[i]  += area0;
    area_tot += area0;
  }

  return area_tot;
}

static void mesh_laplace(real *x1d, real *cot, real *area,
			 /*io*/ real *lbx1d) {
  /*traverse each halfedge and calcuate the
    Laplace-Beltrami operator applied on the position of vextex*. */
  
  int h, n, i, j;
  
  for (h = 0; h < NH; h++) {
    
    n = nxt(h);
    i = ver(h); j = ver(n);
    lbx1d[i] += cot[h]*(x1d[i] - x1d[j])/2;
  }
  
  for (i = 0; i < NV; i++) lbx1d[i] /= area[i];
  
}

static void mesh_norm(/*o*/ real *nx, real *ny, real *nz) {
  /*traverse each triangle to calculate the norm at each vertex

    note: the normal vector on each vertex is an weighted sum of
    all normal vectors for neigboring triangles and the weight
    is the angle associated with the vertex.
    
    Ref: Gukenberger et al Comput. Phys. Comm. 2016. */
  
  enum {X, Y, Z};

  int t, i, j, k, v;
  real a[3], b[3], c[3], n0[3];
  real theta_a, theta_b, theta_c;
  real len;

  for ( t = 0; t < NT; t++ ) {
    
    i = T0[t]; j = T1[t]; k = T2[t];
    
    get3(i, j, k, a, b, c);
    
    theta_a = tri_angle(c, a, b);
    theta_b = tri_angle(a, b, c);
    theta_c = tri_angle(b, c, a);

    tri_normal(a, b, c, n0);
    
    nx[i] += theta_a * n0[X];
    ny[i] += theta_a * n0[Y];
    nz[i] += theta_a * n0[Z];

    nx[j] += theta_b * n0[X];
    ny[j] += theta_b * n0[Y];
    nz[j] += theta_b * n0[Z];

    nx[k] += theta_c * n0[X];
    ny[k] += theta_c * n0[Y];
    nz[k] += theta_c * n0[Z];
    
  }

  /*normalize the normal vectors*/
  for ( v = 0; v < NV; v++ ) {

    vec_get(v, nx, ny, nz, n0);
    len = vec_dot(n0, n0);
    len = sqrt(len);
    nx[v] /= len;
    ny[v] /= len;
    nz[v] /= len;
    
  }
  
}

static real mesh_curvature_mean(real *lbx, real *lby, real *lbz,
				real *nx, real *ny, real *nz,
				real *area, /*io*/
				real *cm ) {
  /*traverse each vertex and compute the mean curvature,
    which is a half the dot product of Laplace-Beltrami operator on position
    and normal vector.

    In then end, return the mean curvature at each vertex and
    total integral of mean curvature over surface area. */
  
  int v;
  real vec1[3], vec2[3], vecdp, cm_intga;

  cm_intga = 0.0;
  
  for ( v = 0; v < NV; v++ ) {
    
    vec_get(v, lbx, lby, lbz, vec1);
    vec_get(v, nx, ny, nz, vec2);
    vecdp = vec_dot(vec1, vec2);
    cm[v] = vecdp / 2.0;

    cm_intga += cm[v] * area[v];
    
  }

  return cm_intga;
  
}

static void mesh_force(real *cot, real *area, real area_tot,
		       real *lbx, real *lby, real *lbz,
		       real *cm, real cm_intga, /*io*/
		       real *fx, real *fy, real *fz) {
  /*traverse each halfedge, we calculate force due to
    1) Helfrich (local) energy with spontaneous curvature
    2) aread-difference elasticity (non-local) energy */
  
  real kB, C0, H0, kAD, D, alpha, Delta_a0;
  int h, n, nn, fnf;
  int i, j, k, l;
  real cot1, area1, rsq;
  real a[3], b[3], c[3], d[3], r[3];
  real lbi[3], lbisq, lbisq_der[3];
  real coef, coef1, coef2;
  real doef, doef1, doef2;
  real da1[3], db1[3], dc[3];
  real da2[3], db2[3], dd[3];
  real df[3];
  /*real curva_mean1;
    real area1_derx, area1_dery, area1_derz;*/

  kB = 1.0;
  /*C0 = -1.0;*/
  C0 = 0.0;
  H0 = C0/2.0;
  kAD= 2.0 * kB / pi;
  D  = 4.0e-3/3.91;
  alpha = kAD*pi/2/area_tot/D/D;
  Delta_a0 = (1+kB/pi/kAD)*D*C0;

  for (h = 0; h < NH; h++) {
    
    n = nxt(h); nn = nxt(n); fnf = flp(nxt(flp(h)));
    i = ver(h); j = ver(n); k = ver(nn); l = ver(fnf);
    
    cot1  = cot[h];
    area1 = area[i];
    
    get4(i, j, k, l, a, b, c, d);
    get_edg(i, j, r);
    rsq = vec_dot(r, r);
    
    vec_get(i, lbx, lby, lbz, lbi);    
    lbisq = vec_dot(lbi, lbi);

    /*###################################
      ###################################
      force part I
      ###################################
      ###################################*/
    
    /*calculate derivative of Laplace-Beltrami operator: part I*/
    /*this is to calculate the derivative of relative position of i, j*/

    coef =  -kB * (cm[i] - H0) * area[i] / 2.0 / cm[i];

    coef1 =  cot1 / area1;
    coef2 = -lbisq * cot1 /area1 / 2.0;
    vec_linear_combination(coef1, lbi, coef2, r, lbisq_der);

    vec_scalar(lbisq_der, coef, df);

    /*accumulate the force on vertices i and j*/
    vec_append(df, i, /**/ fx, fy, fz);
    vec_substr(df, j, /**/ fx, fy, fz);

    /*+++++++++++++++++++++++++++++++++++
      force due to area-difference elasticity: part I
      +++++++++++++++++++++++++++++++++++*/
    
    doef  = -kAD * pi / area_tot * (cm_intga - H0 * area_tot) * area[i] / 2.0 / cm[i];

    vec_scalar(lbisq_der, doef, df);

    /*accumulate the force on vertices i and j*/
    vec_append(df, i, /**/ fx, fy, fz);
    vec_substr(df, j, /**/ fx, fy, fz);

    /*###################################
      ###################################
        force part II
      ###################################
      ###################################*/
    
    /*calculate derivative of Laplace-Beltrami operator: part II*/
    
    /*this is to calculate derivative of cotangent of angle c 
      with respect to poisition b, c, a*/
    
    dtri_cot(b, c, a, /**/ db1, dc, da1);
    
    /*this is calculate derivative of cotangent of angle d 
      with respect to poisition a, d, b*/
    
    dtri_cot(a, d, b, /**/ da2, dd, db2);

    coef1 =  vec_dot(lbi, r) / area1;
    coef2 = -lbisq * rsq / area1 / 4.0;
    coef *= (coef1 + coef2);

    /*accumulate the force on vertices i, j, k*/
    vec_scalar_append(da1, coef, i, /**/ fx, fy, fz);
    vec_scalar_append(db1, coef, j, /**/ fx, fy, fz);
    vec_scalar_append(dc,  coef, k, /**/ fx, fy, fz);

    /*accumulate the force on vertices i, j, l*/
    vec_scalar_append(da2, coef, i, /**/ fx, fy, fz);
    vec_scalar_append(db2, coef, j, /**/ fx, fy, fz);
    vec_scalar_append(dd,  coef, l, /**/ fx, fy, fz);

    /*+++++++++++++++++++++++++++++++++++
      force due to area-difference elasticity: part II
      +++++++++++++++++++++++++++++++++++*/

    doef *= (coef1 + coef2);
    
    /*accumulate the force on vertices i, j, k*/
    vec_scalar_append(da1, doef, i, /**/ fx, fy, fz);
    vec_scalar_append(db1, doef, j, /**/ fx, fy, fz);
    vec_scalar_append(dc,  doef, k, /**/ fx, fy, fz);

    /*accumulate the force on vertices i, j, l*/
    vec_scalar_append(da2, doef, i, /**/ fx, fy, fz);
    vec_scalar_append(db2, doef, j, /**/ fx, fy, fz);
    vec_scalar_append(dd,  doef, l, /**/ fx, fy, fz);
    
    /*###################################
      ###################################
      force part III
      ###################################      
      ###################################*/

    /*calculate derivative of local area with respective to position b, c, a
      This is similiar to the cotangent case above.*/
    coef  = -kB * 2.0 * (cm[i] - H0) * (cm[i] - H0);
    
    coef1 = coef * cot1 / 4.0;
    vec_scalar(r, coef1, df);
    
    /*accumulate the force on vertices i and j*/
    vec_append(df, i, /**/ fx, fy, fz);
    vec_substr(df, j, /**/ fx, fy, fz);
    
    coef2 = coef * rsq / 8.0;
    
    /*accumulate the force on vertices i, j, k*/
    vec_scalar_append(da1, coef2, i, /**/ fx, fy, fz);
    vec_scalar_append(db1, coef2, j, /**/ fx, fy, fz);
    vec_scalar_append(dc,  coef2, k, /**/ fx, fy, fz);

    /*accumulate the force on vertices i, j, l*/
    vec_scalar_append(da2, coef2, i, /**/ fx, fy, fz);
    vec_scalar_append(db2, coef2, j, /**/ fx, fy, fz);
    vec_scalar_append(dd,  coef2, l, /**/ fx, fy, fz);

    
    /*+++++++++++++++++++++++++++++++++++
      force due to area-difference elasticity: part III
      +++++++++++++++++++++++++++++++++++*/

    doef  = -4.0 * kAD * pi / area_tot * (cm_intga - H0 * area_tot) * cm[i];

    doef1 = doef * cot1 / 4.0;
    
    vec_scalar(r, doef1, df);

    /*accumulate the force on vertices i and j*/
    vec_append(df, i, /**/ fx, fy, fz);
    vec_substr(df, j, /**/ fx, fy, fz);    

    doef2 = doef * rsq / 8.0;
    
    /*accumulate the force on vertices i, j, k*/
    vec_scalar_append(da1, doef2, i, /**/ fx, fy, fz);
    vec_scalar_append(db1, doef2, j, /**/ fx, fy, fz);
    vec_scalar_append(dc,  doef2, k, /**/ fx, fy, fz);

    /*accumulate the force on vertices i, j, l*/
    vec_scalar_append(da2, doef2, i, /**/ fx, fy, fz);
    vec_scalar_append(db2, doef2, j, /**/ fx, fy, fz);
    vec_scalar_append(dd,  doef2, l, /**/ fx, fy, fz);

  
  }
    
}
