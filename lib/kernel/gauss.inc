#define gauss_1d (1/sqrt(pi)/erf(1))
#define gauss_2d (1/(2*pi*(1.0/2 - exp(-1)/2)))
#define gauss_3d (1/(exp(-1)*pi*(sqrt(pi)*exp(1)*erf(1) - 2)))

/* 2D and 3D */
static real
gauss_w0(real q)
{								/* 2D and 3D */
	return exp(-q * q);
}

static real
gauss_dw0(real q)
{
	return -2 * q * exp(-q * q);
}

static real
gauss_dwr0(real q)
{
	return -2 * exp(-q * q);
}

static real
gauss_w1(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_1d * gauss_w0(q) / c;
}

static real
gauss_w2(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_2d * gauss_w0(q) / (c * c);
}

static real
gauss_w3(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_3d * gauss_w0(q) / (c * c * c);
}

static real
gauss_dw1(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_2d * gauss_dw0(q) / (c * c);
}

static real
gauss_dw2(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_2d * gauss_dw0(q) / (c * c * c);
}

static real
gauss_dw3(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_3d * gauss_dw0(q) / (c * c * c * c);
}

static real
gauss_dwr1(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_2d * gauss_dwr0(q) / (c * c * c);
}

static real
gauss_dwr2(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_2d * gauss_dwr0(q) / (c * c * c * c);
}

static real
gauss_dwr3(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return gauss_3d * gauss_dwr0(q) / (c * c * c * c * c);
}
