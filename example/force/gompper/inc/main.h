static void main0() {
    real *l2, *A, *T;
    real *LX, *LY, *LZ;
    real *FX, *FY, *FZ, *FM;

    RZERO(NH, &l2); RZERO(NH, &T);
    RZERO(NV, &A);
    RZERO(NV, &LX); RZERO(NV, &LY); RZERO(NV, &LZ);
    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ); RZERO(NV, &FM);

    mesh_t(/**/ T);
    mesh_l2(l2);
    mesh_voronoi(T, l2, /**/ A);

    mesh_laplace(XX, T, A, /**/ LX);
    mesh_laplace(YY, T, A, /**/ LY);
    mesh_laplace(ZZ, T, A, /**/ LZ);

    mesh_force_t (T, LX, LY, LZ, /**/ FX, FY, FZ);
    mesh_force_dt(   LX, LY, LZ, /**/ FX, FY, FZ);
    vabs(NV, FX, FY, FZ, /**/ FM);

    real *queue[] = {RR, ZZ, FM, A, NULL};
    punto_write(NV, queue, "/dev/stdout");

    FREE(l2); FREE(T);
    FREE(A);
    FREE(LX); FREE(LY); FREE(LZ);
    FREE(FX); FREE(FY); FREE(FZ); FREE(FM);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
