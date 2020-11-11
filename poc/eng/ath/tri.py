import compute_shear_energy
import trimesh
import numpy as np

a = [0, 0, 0]
b = [1, 0, 0]
c = [0.5, 1.1, 0]

a0 = [0, 0, 0]
b0 = [1, 0, 0]
c0 = [0, 1, 0]

tri = np.empty((1, 3, 3))
tri0 = np.empty((1, 3, 3))

tri[:, 0] = a
tri[:, 1] = b
tri[:, 2] = c

tri0[:, 0] = a0
tri0[:, 1] = b0
tri0[:, 2] = c0

DAi = trimesh.triangles.area(triangles=tri)
DA0i = trimesh.triangles.area(triangles=tri0)

al, be = compute_shear_energy.compute_invariants(DAi, DA0i, tri, tri0)
#print(al[0], be[0])
b1 = 0.7
b2 = 0.75
#print(compute_shear_energy.compute_shear(b1, b2, al[0], be[0]))
a3 = 2
a4 = -8

print(compute_shear_energy.compute_area(a3, a4, al[0]))
