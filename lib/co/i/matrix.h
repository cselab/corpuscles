static real
i_matrix_get(int M, int N, int m, int n, const real * a)
{
    return a[N * m + n];
}

static int
i_matrix_set(int M, int N, int m, int n, real s, real * a)
{
    a[N * m + n] = s;
    return CO_OK;
}

static int
i_matrix_scale(int M, int N, real s, real * a)
{
    int i;

    for (i = 0; i < N * M; i++)
        a[i] *= s;
    return CO_OK;
}

static int
i_matrix_add(int M, int N, int m, int n, real s, real * a)
{
    a[N * m + n] += s;
    return CO_OK;
}
