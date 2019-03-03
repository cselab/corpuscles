	r=`co.path`/sph/icosahedron/1.off
	i=`co.path`/rbc/icosahedron/1.off
	./main strain $r lim 0 1 0 0 0 0 < $i  > q.vtk


from `gerris-stable/tools/gfs2oogl.c`

	fprintf (fp, "VECT 1 3 0 3 0 %g %g %g %g %g %g %g %g %g\n",
		 x + u - (u - v/2.)/5.,
		 y + v - (u/2. + v)/5.,
		 z + w,
		 x + u,
		 y + v,
		 z + w,
		 x + u - (u + v/2.)/5.,
		 y + v + (u/2. - v)/5.,
		 z + w);
	fprintf (fp, "VECT 1 2 0 2 0 %g %g %g %g %g %g\n",
		 x, y, z,
		 x + u,
		 y + v,
		 z + w);
