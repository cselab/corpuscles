#define yang_1d 1.0/3
#define yang_2d 10.0/(7*pi)
#define yang_3d 2.0/pi

/* 2D and 3D */
static real
yang_w0(real q)
{								/* 2D and 3D */
	if (q < 1)
		return pow(2 - q, 3) - 4 * pow(1 - q, 3);
	else if (q < 2)
		return pow(2 - q, 3);
	else
		return 0;
}

static real
yang_dw0(real q)
{
	if (q < 1)
		return 12 * pow(1 - q, 2) - 3 * pow(2 - q, 2);
	else if (q < 2)
		return -3 * pow(2 - q, 2);
	else
		return 0;
}

static real
yang_dwr0(real q)
{
	if (q < 1)
		return 3 * (3 * q - 4);
	else if (q < 2)
		return -3 * pow(2 - q, 2) / q;
	else
		return 0;
}

static real
yang_w1(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return yang_1d * yang_w0(q) / c;
}

static real
yang_w2(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return yang_2d * yang_w0(q) / (c * c);
}

static real
yang_w3(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return yang_3d * yang_w0(q) / (c * c * c);
}

static real
yang_dw1(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return 2 * yang_2d * yang_dw0(q) / (c * c);
}

static real
yang_dw2(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return 2 * yang_2d * yang_dw0(q) / (c * c * c);
}

static real
yang_dw3(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return 2 * yang_3d * yang_dw0(q) / (c * c * c * c);
}

static real
yang_dwr1(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return 4 * yang_2d * yang_dwr0(q) / (c * c * c);
}

static real
yang_dwr2(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return 4 * yang_2d * yang_dwr0(q) / (c * c * c * c);
}

static real
yang_dwr3(real c, real x)
{
	real q;

	q = 2 * fabs(x) / c;
	return 4 * yang_3d * yang_dwr0(q) / (c * c * c * c * c);
}
