# Run

	make run

# Notation

`r[i, X]`, `r[i, Y]`, `r[i, Z]`: vertex coordinates

`nt`, `nv`, `nh = 3*nv`: number of triangles, number of vertices, number of half-edges

`BEGIN_T/END_T`: visit every triangles three times, sets `i`, `j`, `k`, `a`, `b`, `c`

`i`, `j`, `k`: vertex indexes

`a`, `b`, `c`: vertex coordinates

`tb`, `tc`: cotangent

`u`: triangle normal

`ang`: angle

`n[i]`: vertex normal

`K`, `H`: Gaussian and mean curvatures

`lpH`: Laplace operator of curvatures

`fm`: absolute force density