static void write(real *fx, real *fy, real *fz,
                  real *lx, real *ly, real *lz,
                  real *A) {
    real *fm, *lm;
    RZERO(NV, &fm); RZERO(NV, &lm);

    vabs(NV, fx, fy, fz, /**/ fm);
    vabs(NV, lx, ly, lz, /**/ lm);

    printf("#1 azimuth angle; 2 axis dist; 3 zz; 4 |F|; 5 fx; 6 fy; 7 fz; 8 area; 9; |laplace-beltrami|\n");
    real *queue[] = {TH, RR, ZZ, fm, fx, fy, fz, A, lm, NULL};
    punto_fwrite(NV, queue, stdout);

    FREE(lm);  FREE(fm);
}

static void main0() {
    real *l2, *A, *T;
    real *LX, *LY, *LZ;
    real *FX, *FY, *FZ;

    RZERO(NH, &l2); RZERO(NH, &T);
    RZERO(NV, &A);
    RZERO(NV, &LX); RZERO(NV, &LY); RZERO(NV, &LZ);
    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);

    mesh_t(/**/ T);
    mesh_l2(l2);
    mesh_voronoi(T, l2, /**/ A);

    mesh_laplace(XX, T, A, /**/ LX);
    mesh_laplace(YY, T, A, /**/ LY);
    mesh_laplace(ZZ, T, A, /**/ LZ);

    mesh_force_t (T, LX, LY, LZ, /*io*/ FX, FY, FZ);
    mesh_force_dt(   LX, LY, LZ, /*io*/ FX, FY, FZ);

    write(FX, FY, FZ, LX, LY, LZ, A);

    FREE(l2); FREE(T);
    FREE(A);
    FREE(LX); FREE(LY); FREE(LZ);
    FREE(FX); FREE(FY); FREE(FZ);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return CO_OK;
}
