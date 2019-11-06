#include <string.h>
#include <math.h>
#include <stdio.h>

#include <real.h>
#include <co/distmesh.h>
#include <co/err.h>
#include <co/he.h>
#include <co/memory.h>
#include <co/off.h>
#include <co/y.h>

static real
min(real x, real y)
{
	return x < y ? x : y;
}

static real
sqr(real x)
{
	return x * x;
}

static real
length(const real * x)
{
	return sqrt(sqr(x[0]) + sqr(x[1]) + sqr(x[2]));
}

static real
dot(const real * x, const real * y)
{
	return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

void
cross(const real * v1, const real * v2, real * n)
{
	n[0] = v1[1] * v2[2] - v1[2] * v2[1];
	n[1] = v1[2] * v2[0] - v1[0] * v2[2];
	n[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

static const int mod3x1[3] = { 1, 2, 0 };
static const int mod3x2[3] = { 2, 0, 1 };

static void
trinormal3(const real * p1, const real * p2, const real * p3, real * n)
{
	real d12[3] = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
	real d13[3] = { p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2] };
	cross(d12, d13, n);

	real nnorm = length(n);

	n[0] /= nnorm;
	n[1] /= nnorm;
	n[2] /= nnorm;
}

static real
triqual3(const real * p1, const real * p2, const real * p3)
{
	real n[3];
	real d12[3] = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
	real d13[3] = { p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2] };
	real d23[3] = { p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2] };
	cross(d12, d13, n);
	real vol = length(n) / 2.0;
	real den = dot(d12, d12) + dot(d13, d13) + dot(d23, d23);

	return 6.928203230275509 * vol / den;
}

static void
tupdate(const real * p, int nt, int *t, int *t2t, int *t2n)
{
    int t1, n1, n2, t2, tix11, tix12, tix21, tix22, flip, nbt, nbn;
    real q1, q2, q3, q4, minqold, minqnew;

	for (t1 = 0; t1 < nt; t1++)
		for (n1 = 0; n1 < 3; n1++) {
			int t2 = t2t[n1 + 3 * t1];

			if (t2 >= 0) {
				q1 =
					triqual3(&p[3 * t[0 + 3 * t1]], &p[3 * t[1 + 3 * t1]],
							 &p[3 * t[2 + 3 * t1]]);
				q2 =
					triqual3(&p[3 * t[0 + 3 * t2]], &p[3 * t[1 + 3 * t2]],
							 &p[3 * t[2 + 3 * t2]]);
				minqold = min(q1, q2);

				if (minqold < 0.9) {
					n2 = t2n[n1 + 3 * t1];
					tix11 = mod3x1[n1];
					tix12 = mod3x2[n1];
					tix21 = mod3x1[n2];
					tix22 = mod3x2[n2];

					int newt[2][3] =
						{ t[0 + 3 * t1], t[1 + 3 * t1], t[2 + 3 * t1],
						t[0 + 3 * t2], t[1 + 3 * t2], t[2 + 3 * t2]
					};

					/* Swap edge */
					newt[0][tix12] = newt[1][n2];
					newt[1][tix22] = newt[0][n1];

					q3 =
						triqual3(&p[3 * newt[0][0]], &p[3 * newt[0][1]],
								 &p[3 * newt[0][2]]);
					q4 =
						triqual3(&p[3 * newt[1][0]], &p[3 * newt[1][1]],
								 &p[3 * newt[1][2]]);
					minqnew = min(q3, q4);

					if (minqnew > minqold + .025) {
						real normal1[3], normal2[3], normal3[3],
							normal4[3];
						trinormal3(&p[3 * t[0 + 3 * t1]],
								   &p[3 * t[1 + 3 * t1]],
								   &p[3 * t[2 + 3 * t1]], normal1);
						trinormal3(&p[3 * t[0 + 3 * t2]],
								   &p[3 * t[1 + 3 * t2]],
								   &p[3 * t[2 + 3 * t2]], normal2);
						trinormal3(&p[3 * newt[0][0]], &p[3 * newt[0][1]],
								   &p[3 * newt[0][2]], normal3);
						trinormal3(&p[3 * newt[1][0]], &p[3 * newt[1][1]],
								   &p[3 * newt[1][2]], normal4);
						flip = dot(normal1, normal2) > 0
							&& dot(normal3, normal4) > 0;
						if (flip) {
							/* Insert new triangles */
							memcpy(t + 3 * t1, newt[0], 3 * sizeof(int));
							memcpy(t + 3 * t2, newt[1], 3 * sizeof(int));

							/* Update t2t and t2n */
							nbt = t2t[tix21 + 3 * t2];
							nbn = t2n[tix21 + 3 * t2];
							t2t[n1 + 3 * t1] = nbt;
							t2n[n1 + 3 * t1] = nbn;
							if (nbt >= 0) {
								t2t[nbn + 3 * nbt] = t1;
								t2n[nbn + 3 * nbt] = n1;
							}

							nbt = t2t[tix11 + 3 * t1];
							nbn = t2n[tix11 + 3 * t1];
							t2t[n2 + 3 * t2] = nbt;
							t2n[n2 + 3 * t2] = nbn;
							if (nbt >= 0) {
								t2t[nbn + 3 * nbt] = t2;
								t2n[nbn + 3 * nbt] = n2;
							}

							t2t[tix11 + 3 * t1] = t2;
							t2n[tix11 + 3 * t1] = tix21;
							t2t[tix21 + 3 * t2] = t1;
							t2n[tix21 + 3 * t2] = tix11;
						}
					}
				}
			}
		}
}


int
main()
{
	He *he;
	real *x, *y, *z, *p;
	int i, j, k, m, t, nt, nv;
	int *t2t, *t2n, *tri, *tri0;

	y_inif(stdin, &he, &x, &y, &z);
	nt = he_nt(he);
	nv = he_nv(he);
	MALLOC2(3 * nt, &t2t, &t2n);
	MALLOC2(3 * nt, &tri, &tri0);
	MALLOC(3 * nv, &p);
	for (m = i = 0; i < nv; i++) {
		p[m++] = x[i];
		p[m++] = y[i];
		p[m++] = z[i];
	}
	for (m = t = 0; t < nt; t++) {
		he_tri_ijk(he, t, &i, &j, &k);
		tri[m++] = i;
		tri[m++] = j;
		tri[m++] = k;
	}

	for (i = 0; i < 3 * nt; i++)
		tri0[i] = tri[i];

	distmesh_t2t(he, t2t);
	distmesh_t2n(he, t2n);

	tupdate(p, nt, tri, t2t, t2n);
	for (i = 0; i < 3 * nt; i++)
		if (tri[i] != tri0[i])
			MSG("tri[%d]=%d != tri0[%d]=%d", i, tri[i], i, tri0[i]);

	off_xyz_tri_fwrite(nv, p, nt, tri, stdout);

	FREE2(t2t, t2n);
	FREE2(tri, tri0);
	FREE(p);
	y_fin(he, x, y, z);
	return CO_OK;
}
