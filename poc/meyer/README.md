# Run

	make run

# Notation

i, j, k: vertex indexes
a, b, c: vertex coordinates
nt, nv, nh = 3*nv: number of triangles, number of vertices, number of half-edges

BEGIN_T/END_T: visit every triangles three times, sets i, j, k, a, b, c
BEGIN_V/END_V: visit every vetex

tb, tc: cotangent
u: triangle normal
ang: angle
n[i]: vetrex normal
K, H: gaussian and mean curvatures
lpH: laplace operator of curvatures
fm: absolute value of force
