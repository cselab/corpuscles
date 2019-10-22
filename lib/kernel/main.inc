#define T Kernel

typedef real(*Fun) (real, real);

static Fun w[][3] = {
	{ gauss_w1, gauss_w2, gauss_w3 },
	{ NULL, quintic_w2, quintic_w3 },
	{ wendland6_w1, wendland6_w2, wendland6_w3 },
	{ yang_w1, yang_w2, yang_w3 }
};

static Fun dw[][3] = {
	{ gauss_dw1, gauss_dw2, gauss_dw3 },
	{ NULL, quintic_dw2, quintic_dw3 },
	{ wendland6_dw1, wendland6_dw2, wendland6_dw3 },
	{ yang_dw1, yang_dw2, yang_dw3 }
};

static Fun dwr[][3] = {
	{ gauss_dwr1, gauss_dwr2, gauss_dwr3 },
	{ NULL, quintic_dwr2, quintic_dwr3 },
	{ wendland6_dwr1, wendland6_dwr2, wendland6_dwr3 },
	{ yang_dwr1, yang_dwr2, yang_dwr3 }
};

static int Type[] =
	{ KERNEL_GAUSS, KERNEL_QUINTIC, KERNEL_WENDLAND6, KERNEL_YANG };
static char *String[] = { "gauss", "quintic", "wendland6", "yang" };

struct T {
	int dim;
	Fun w, dw, dwr;
};

int
kernel_ini(int dim, int type, T ** pq)
{
	T *q;
	int i, n;

	switch (dim) {
	case (KERNEL_1D):
		break;
	case (KERNEL_2D):
		break;
	case (KERNEL_3D):
		break;
	default:
		ERR(CO_NUM,
			"unknown dimenshon, possible values are KERNEL_1D, KERNEL_2D, KERNEL_3D");
	}

	i = 0;
	n = sizeof(Type) / sizeof(Type[0]);
	for (;;) {
		if (type == Type[i])
			break;
		if (i == n)
			ERR(CO_INDEX, "unknown kernal type: '%d'", type);
		i++;
	}

	MALLOC(1, &q);
	q->dim = dim;
	q->w = w[type][dim];
	q->dw = dw[type][dim];
	q->dwr = dwr[type][dim];
	*pq = q;
	return CO_OK;
}

int
kernel_argv(char ***p, T ** pq)
{
	int status, n, i;
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
	else
		ERR(CO_NUM, "unknown dimenshion '%d'", dim);

	if ((status = argv_str(p, stype)) != CO_OK)
		return status;

	i = 0;
	n = sizeof(Type) / sizeof(Type[0]);
	for (;;) {
		if (util_eq(stype, String[i])) {
			type = Type[i];
			break;
		}
		if (i == n)
			ERR(CO_INDEX, "unknown kernal type: '%s'", stype);
		i++;
	}
	return kernel_ini(dim, type, pq);
}


int
kernel_fin(T * q)
{
	FREE(q);
	return CO_OK;
}

real
kernel_w(T * q, real c, real x)
{
	return q->w(c, x);
}

real
kernel_dw(T * q, real c, real x)
{
	return q->dw(c, x);
}

real
kernel_dwr(T * q, real c, real x)
{
	return q->dwr(c, x);
}
