#include <stdio.h>
#include <stdlib.h>
#include <real.h>

#include <co/arg.h>
#include <co/err.h>
#include <co/kernel.h>

const char* me= "kernal";

#define FMT CO_REAL_OUT

#define pi (3.1415926)

void
usg(void)
{
	fprintf(stderr, "%s: -c cutoff -l low -h hi -n [number of points]\n", me);
}

char *argv0;
int
main(int argc, char **argv)
{
	real c, l, h, x, y, dy, dyx, dx, s;
	int i, n, Int1, Int2, Int3;
	l = 0;
	h = 1;
	c = 1;
	n = 20;
	Int1 = Int2 = Int3 =  0;
	ARGBEGIN {
	case '1':
		Int1 = 1;
		break;
	case '2':
		Int2 = 1;
		break;
	case '3':
		Int3 = 1;
		break;
	case 'c':
		c = strtod(EARGF(usg()), NULL);
		break;
	case 'l':
		l= strtod(EARGF(usg()), NULL);
		break;
	case 'h':
		h = strtod(EARGF(usg()), NULL);
		break;
	case 'n':
		n = strtol(EARGF(usg()), NULL, 0);
		break;
	default:
		ER("unknow argumen: argv[0]");
	}
	ARGEND;
	Kernel *kernel;
	kernel_argv(&argv, &kernel);

	dx = (h - l)/(n - 1);
	if (Int1) {
		for (i = 0; i < n; i++) {
			x = l + dx*i;
			y = kernel_w(kernel, c, x);
			s += y;
		}
		s *= 2*dx;
		printf(FMT "\n", s);
	} else if (Int2) {
		for (i = 0; i < n; i++) {
			x = l + dx*i;
			y = kernel_w(kernel, c, x);
			s += x*y;
		}
		s *= 2*pi*dx;
		printf(FMT "\n", s);
	} else if (Int3) {
		for (i = 0; i < n; i++) {
			x = l + dx*i;
			y = kernel_w(kernel, c, x);
			s += x*x*y;
		}
		s *= 4*pi*dx;
		printf(FMT "\n", s);
	} else {
		for (i = 0; i < n; i++) {
			x = l + dx*i;
			y = kernel_w(kernel, c, x);
			dy = kernel_dw(kernel, c, x);
			dyx = kernel_dwr(kernel, c, x);
			printf(FMT " " FMT " " FMT " " FMT "\n", x, y, dy, dyx);
		}
	}

	kernel_fin(kernel);
	return CO_OK;
}

/*

make
gnuplot -p <<'!'
set style data lp
f = "<co.run ./main -n 100 -l -3 -h 4 -c 2 3 quintic"
p f u 1:2, "" u 1:3
!
*/
