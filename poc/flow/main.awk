#!/bin/sh

${AWK=awk} '
BEGIN {
	lo = -5
	hi = 5
	n = 20
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			x = lo + (hi - lo)*i/(n - 1)
			y = lo + (hi - lo)*j/(n - 1)
			z = 0
			vel(x, y, z)
			print x, y, vx, vy
		}
	}
}
function vel(x, y, z,   r,  dx, dy, dz, nx, ny, nz) {
	r = sqrt(x^2 + y^2 + z^2)
	if (r < 1/2) {
		vx = vy = vz = 0
		return
	}
	r = 1/r
	dx = x^2*r^2
	dy = x*y*r^2
	dz = x*z*r^2
	vx = -3/4*r*(1 + dx) - 1/4*r^3*(1 - 3*dx) + 1
	vy = -3/4*r*dy          - 1/4*r^3*(-3*dy)
	vz = -3/4*r*dz          - 1/4*r^3*(-3*dz)
}
'