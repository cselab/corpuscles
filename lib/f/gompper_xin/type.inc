struct T {
    real Kb;

    int *T0, *T1, *T2;
    real *l2, *t;

    real *lbx, *lby, *lbz;
    real *normx, *normy, *normz;
    real *H, *curva_gauss;
    real *energy, *area;

    real *fx, *fy, *fz;

    int nv, ne, nt;
};
