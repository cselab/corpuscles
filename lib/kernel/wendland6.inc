#define wendland6_1d (55/32.0)
#define wendland6_2d (78/(7*pi))
#define wendland6_3d (1365/(64*pi))

/* 1D */
static real
wendland6_w1(real c, real x)
{
	real q, a;

	q = fabs(x) / c;
	if (q < 1) {
		a = pow(1 - q, 7) * (21 * pow(q, 3) + 19 * pow(q, 2) + 7 * q + 1);
		return a * wendland6_1d / c;
	} else
		return 0;
}

static real
wendland6_dw1(real c, real x)
{
	real q, a;

	q = fabs(x) / c;
	if (q < 1) {
		a = -6 * pow(q - 1, 6) * q * (35 * pow(q, 2) + 18 * q + 3);
		return a * wendland6_1d / (c * c);
	} else
		return 0;
}

static real
wendland6_dwr1(real c, real x)
{
	return wendland6_dw1(c, x) / fabs(x);
}

/* 2D and 3D */
static real
wendland6_w0(real q)
{								/* 2D and 3D */
	if (q < 1)
		return pow(1 - q,
				   8) * (32 * pow(q, 3) + 25 * pow(q, 2) + 8 * q + 1);
	else
		return 0;
}

static real
wendland6_dw0(real q)
{
	if (q < 1)
		return 22 * pow(q - 1, 7) * (16 * pow(q, 2) + 7 * q + 1);
	else
		return 0;
}

static real
wendland6_w2(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return wendland6_2d * wendland6_w0(q) / (c * c);
}

static real
wendland6_w3(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return wendland6_3d * wendland6_w0(q) / (c * c * c);
}

static real
wendland6_dw2(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return wendland6_2d * wendland6_dw0(q) / (c * c * c) * q;
}

static real
wendland6_dw3(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return wendland6_3d * wendland6_dw0(q) / (c * c * c * c) * q;
}

static real
wendland6_dwr2(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return wendland6_2d * wendland6_dw0(q) / (c * c * c * c);
}

static real
wendland6_dwr3(real c, real x)
{
	real q;

	q = fabs(x) / c;
	return wendland6_3d * wendland6_dw0(q) / (c * c * c * c * c);
}
