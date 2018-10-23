struct T {
    real Kb;
    real *l2, *t;

    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H, *curva_gauss;
    real *energy, *area;

    real *f2x, *f2y, *f2z;

    int nv, ne, nt;
};
