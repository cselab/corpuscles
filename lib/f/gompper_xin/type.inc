struct T {
    real Kb, H0;
    
    real *l2, *t;

    real *lbx, *lby, *lbz;
    real *lbx0, *lby0, *lbz0;

    real *normx, *normy, *normz;
    real *H, *curva_gauss;
    real *energy, *area;

    real *f1x, *f1y, *f1z;
    real *f2x, *f2y, *f2z;

    int nv, ne, nt;
};
