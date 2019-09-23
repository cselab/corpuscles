static real
matrix_get(int M, int N, int m, int n, const real *a)
{
    return a[N*m + n];
}

static int
matrix_set(int M, int N, int m, int n, real s, real *a)
{
	a[N*m + n] = s;
	return CO_OK;
}
