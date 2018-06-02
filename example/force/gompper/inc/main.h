static void main0() {
    real *cot, *l2, *A, *T;
    real *LX, *LY, *LZ;
    real *FX, *FY, *FZ;

    RZERO(NH, &cot); RZERO(NH, &l2); RZERO(NH, &T);
    RZERO(NV, &A);
    RZERO(NV, &LX); RZERO(NV, &LY); RZERO(NV, &LZ);
    RZERO(NV, &FX); RZERO(NV, &FY); RZERO(NV, &FZ);

    real *queue[] = {RR, FX, FY, FZ, NULL};

    mesh_cot(cot);
    mesh_T(cot, /**/ T);
    mesh_l2(l2);
    mesh_voronoi(cot, l2, /**/ A);

    mesh_laplace(XX, T, A, /**/ LX);
    mesh_laplace(YY, T, A, /**/ LY);
    mesh_laplace(ZZ, T, A, /**/ LZ);

    MSG("%g", sum(NV, A));
    punto_write(NV, queue, "/dev/stdout");

    FREE(cot); FREE(l2); FREE(T);
    FREE(A);
    FREE(LX); FREE(LY); FREE(LZ);
    FREE(FX); FREE(FY); FREE(FZ);
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
