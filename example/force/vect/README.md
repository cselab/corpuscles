	r=`he.path`/sph/icosahedron/1.off
	i=`he.path`/rbc/icosahedron/1.off
	./main strain $r lim 0 1 0 0 0 0 < $i  > q.vtk


from `gerris-stable/tools/gfs2oogl.c`

	fprintf (fp, "VECT 1 3 0 3 0 %g %g %g %g %g %g %g %g %g\n",
		 pos.x + f.x - (f.x - f.y/2.)/5.,
		 pos.y + f.y - (f.x/2. + f.y)/5.,
		 pos.z + f.z,
		 pos.x + f.x,
		 pos.y + f.y,
		 pos.z + f.z,
		 pos.x + f.x - (f.x + f.y/2.)/5.,
		 pos.y + f.y + (f.x/2. - f.y)/5.,
		 pos.z + f.z);
	fprintf (fp, "VECT 1 2 0 2 0 %g %g %g %g %g %g\n",
		 pos.x, pos.y, pos.z,
		 pos.x + f.x,
		 pos.y + f.y,
		 pos.z + f.z);
