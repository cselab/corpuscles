    r=`he.path`/sph/icosahedron/1.off
    i=`he.path`/rbc/icosahedron/1.off
    ./main strain $r lim 0 1 0 0 0 0 < $i  > q.vtk
