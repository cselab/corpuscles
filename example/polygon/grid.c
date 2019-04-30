#include <stdio.h>
#include <real.h>

#include <co/memory.h>
#include <co/polygon.h>
#include <co/punto.h>
#include <co/skel.h>

#define FMT CO_REAL_OUT

int
main(int argc, const char **argv)
{
	Polygon *polygon;
	Skel *skel;
	real *x, *y, *u, *v, *color;

	int m, n, i, j, l;
	real lo, hi, dx, x0, y0;
	real clo = -2;
	real chi = 2;
	real size = 0.2;

	m  = 50;
	lo = -1;
	hi =  1;
	n = m*m;
	MALLOC(n, &u);
	MALLOC(n, &v);
	CALLOC(n, &color);

	skel_read(stdin, &x, &y, &skel);
	polygon_ini(clo, chi, size, &polygon);
	polygon_update(polygon, skel, x, y);

	dx = (hi - lo)/(m - 1);
	for (l = i = 0; i <m ; i++) {
		x0 = lo + dx*i;
		for (j = 0; j < m; j++) {
			y0 = lo + dx*j;
			u[l] = x0;
			v[l] = y0;
			color[l] = polygon_inside_fast(polygon, u[l], v[l]);
			l++;
		}
	}

	const real *q[] = {
		u, v, color, NULL
	};
	punto_fwrite(n, q, stdout);

	FREE(u); 
	FREE(v); 
	FREE(color);
	skel_xy_fin(x, y, skel);
	polygon_fin(polygon);
}

/*

m clean lint
valgrind ./grid < data/rbc.skel > q

gp <<'!'
plot "<awk '$3 == 1 'q", "<awk '$3 == 0' q"
!

*/