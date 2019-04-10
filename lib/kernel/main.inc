#define T Kernel

typedef real (*Fun)(real, real);
static Fun   w[][3] = {
	{NULL,         quintic_w2,   quintic_w3},
	{wendland6_w1,  wendland6_w2, wendland6_w3},
	{yang_w1,  yang_w2, yang_w3}
};

static Fun  dw[][3] = {
	{        NULL,  quintic_dw2,    quintic_dw3},
	{wendland6_dw1, wendland6_dw2, wendland6_dw3},
	{yang_dw1, yang_dw2, yang_dw3}
};

static Fun  dwr[][3] = {
	{        NULL,  quintic_dwr2,    quintic_dwr3},
	{wendland6_dwr1, wendland6_dwr2, wendland6_dwr3},
	{yang_dwr1, yang_dwr2, yang_dwr3}
};


struct T {
	int dim;
	Fun w, dw, dwr;
};

int
kernel_ini(int dim, int type, T **pq)
{
	T *q;
	switch (dim) {
		case(KERNEL_1D):
		break;
		case(KERNEL_2D):
		break;
		case(KERNEL_3D):
		break;
	default:
		ERR(CO_NUM, "unknown dimenshon, possible values are KERNEL_1D, KERNEL_2D, KERNEL_3D");
	}

	MALLOC(1, &q);
	switch (type) {
	case (KERNEL_QUINTIC):
		break;
	case (KERNEL_WENDLAND6):
		break;
	case (KERNEL_YANG):
		break;
	default:
		ERR(CO_NUM, "unknown type, possible values are KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG");
	}

	q->dim = dim;
	q->w = w[type][dim];
	q->dw = dw[type][dim];
	q->dwr = dwr[type][dim];
	*pq = q;
	return CO_OK;
}

int
kernel_argv(char ***p,  T **pq)
{
	int status;
	char stype[999];
	int dim, type;
	if ((status = argv_int(p, &dim)) != CO_OK)
		return status;

	if (dim == 1)
		dim = KERNEL_1D;
	else if (dim == 2)
		dim = KERNEL_2D;
	else if (dim == 3)
		dim = KERNEL_3D;
	else ERR(CO_NUM, "unknown dimenshion '%d'", dim);

	if ((status = argv_str(p, stype)) != CO_OK)
		return status;

	if (util_eq(stype, "wendland6"))
		type = KERNEL_WENDLAND6;
	else if (util_eq(stype, "quintic"))
		type = KERNEL_QUINTIC;
	else if (util_eq(stype, "yang"))
		type = KERNEL_YANG;
	else
		ERR(CO_INDEX, "unknown kernal type: '%s'", stype);

	return kernel_ini(dim, type, pq); 
}


int
kernel_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

real
kernel_w(T *q, real c, real x)
{
	return q->w(c, x);
}

real 
kernel_dw(T *q, real c, real x)
{
	return q->dw(c, x);
}

real 
kernel_dwr(T *q, real c, real x)
{
	return q->dwr(c, x);
}
