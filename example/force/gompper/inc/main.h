static void main0() {
    real *cot, *l2, *A, *T, *LX, *LY, *LZ;

    RZERO(NH, &cot); RZERO(NH, &l2); RZERO(NH, &T);
    RZERO(NV, &A);
    RZERO(NV, &LX); RZERO(NV, &LY); RZERO(NV, &LZ);

    real *queue[] = {RR, LX, LY, LZ, NULL};

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
}

int main() {
    ini("/dev/stdin");
    main0();
    fin();
    return HE_OK;
}
