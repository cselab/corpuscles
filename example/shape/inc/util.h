static void get3(int i, int j, int k, /**/
		 real a[3], real b[3], real c[3]) {
  /*Given three indices i, j, k of vertices,
    return their positions in a, b, c.*/
  
  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_get(k, XX, YY, ZZ, c);
  
}

void get4(int i, int j, int k, int l, /**/
          real a[3], real b[3], real c[3], real d[3]) {

  /*Given four indices i, j, k l of vertices,
    return their positions in a, b, c, d.*/

  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_get(k, XX, YY, ZZ, c);
  vec_get(l, XX, YY, ZZ, d);
  
}

static void get_edg(int i, int j, real r[3]) {
  /*Given the two indices i, j of vertices,
    return the edge as a vector.*/
  
  real a[3], b[3];
  
  vec_get(i, XX, YY, ZZ, a);
  vec_get(j, XX, YY, ZZ, b);
  vec_minus(a, b, r);
    
}

static void data_copy(real *a, real *b, int num) {
  /*Given 1D array of data stored in a[],
    copy them into b[].

  note that we do not check if dimensions are valid,
  which should be improved.*/
  
  int i;
  
  for ( i=0; i < num; i ++ )  {
    
    b[i] = a[i];
    
  }
  
}

static void data_reset(real *a, int num) {
  /*Given 1D array of data stored in a[],
    reset them to zeros.*/

  int i;
  
  for ( i=0; i < num; i ++ )  {
    
    a[i] = 0;
    
  }
  
}
