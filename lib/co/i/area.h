typedef struct He He;
static int
i_area_ver(He * he, const real * x, const real * y, const real * z,
           /**/ real * area)
{
    int nv, nt, t;
    int i, j, k;
    int *t0, *t1, *t2;
    real area0;
    real a[3], b[3], c[3];

    nt = he_nt(he);
    nv = he_nv(he);
    he_T(he, &t0, &t1, &t2);

    for (i = 0; i < nv; i++)
        area[i] = 0;

    for (t = 0; t < nt; t++) {
        i = t0[t];
        j = t1[t];
        k = t2[t];
        i_vec_get3(i, j, k, x, y, z, a, b, c);
        area0 = i_tri_area(a, b, c) / 3;
        area[i] += area0;
        area[j] += area0;
        area[k] += area0;
    }
    return CO_OK;
}
