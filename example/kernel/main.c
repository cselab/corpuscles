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
	real c, l, h, x, y, dy, dx, s;
	int i, n, Int2;
	l = 0;
	h = 1;
	c = 1;
	n = 20;
	Int2 = 0;
	ARGBEGIN {
	case '2':
		Int2 = 1;
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

	if (Int2) {
		dx = (h - l)/(n - 1);
		for (i = 0; i < n; i++) {
			x = l + dx*i;
			y = kernel_w(kernel, c, x);
			s += x*x*y;
		}
		s *= 2*pi*dx;
		printf(FMT "\n", s);
	} else {
		for (i = 0; i < n; i++) {
			x = l + (h - l)*i/(n - 1);
			y = kernel_w(kernel, c, x);
			dy = kernel_dw(kernel, c, x);
			printf(FMT " " FMT " " FMT "\n", x, y, dy);
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