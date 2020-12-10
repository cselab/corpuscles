import math
import numpy as np
import trimesh
import argparse
import matplotlib.pyplot as plt


def single_angle_between(v1, v2):
    x = np.dot(v1, v2)
    n = np.cross(v1, v2)
    y = np.sqrt(np.dot(n, n))
    return math.atan2(y, x)


def angle_between(vec1, vec2):
    angles = np.zeros(len(vec1))
    for i, v1, v2 in zip(range(len(vec1)), vec1, vec2):
        angles[i] = single_angle_between(v1, v2)
    return angles


def single_compute_magnitude(v):
    return np.sqrt( v.dot(v) )


def compute_magnitude(vec):
    mag = np.zeros(len(vec))
    for i, v in enumerate(vec):
        mag[i] = single_compute_magnitude(v)
    return mag



def compute_invariants(DAi, DA0i, tri, tri0):

    alpha = DAi/DA0i - 1

    # vertex points
    v1  = tri [:,0]
    v2  = tri [:,1]
    v3  = tri [:,2]
    w1  = tri0[:,0]
    w2  = tri0[:,1]
    w3  = tri0[:,2]

    # vectors on triangle edge
    l   = v2 - v1
    lp  = v3 - v1
    l0  = w2 - w1
    lp0 = w3 - w1

    Ml   = compute_magnitude(l)
    Ml0  = compute_magnitude(l0)
    Mlp  = compute_magnitude(lp)
    Mlp0 = compute_magnitude(lp0)

    a = Ml / Ml0

    phi  = angle_between(l,  lp)
    phi0 = angle_between(l0, lp0)

    b = 1./np.sin(phi0) * ( Mlp/Mlp0*np.cos(phi) - Ml/Ml0*np.cos(phi0) )

    c = Mlp/Mlp0 * np.sin(phi)/np.sin(phi0)

    a2 = a*a
    b2 = b*b
    c2 = c*c
    a2c = 2.*a*c
    beta = 1./a2c * ( a2 + b2 + c2 - a2c )

    return alpha, beta


def compute_Eshear(b1, b2, ai, bi):
    ai2 = ai  * ai
    ai3 = ai2 * ai
    ai4 = ai2 * ai2
    bi2 = bi  * bi
    return bi  + b1*ai*bi + b2*bi2


def compute_Earea(a3, a4, ai):
    ai2 = ai  * ai
    ai3 = ai2 * ai
    ai4 = ai2 * ai2
    return 0.5*(ai2 + a3*ai3 + a4*ai4)




if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('--ref',        type=str,   required=True,  help="stress-free mesh file")
    parser.add_argument('--f',      type=str,   required=True,  nargs='+', help="rbc mesh files")

    parser.add_argument('--L_UNIT',     type=float, default=3.33779e-6, help="length unit, m")
    parser.add_argument('--mu',         type=float, default=2.5e-6, help="shear modulus")
    parser.add_argument('--b1',         type=float, default=0.7,    help="non-linear shear coefficients")
    parser.add_argument('--b2',         type=float, default=0.75,   help="non-linear shear coefficients")
    parser.add_argument('--ka',         type=float, default=5.0e-6, help="stretch modulus")
    parser.add_argument('--a3',         type=float, default=-2,     help="non-linear stretch coefficients")
    parser.add_argument('--a4',         type=float, default=8,      help="non-linear stretch coefficients")
    parser.add_argument('--kb',         type=float, default=2.e-19, help="bending modulus")
    parser.add_argument('--m0bar',      type=float, default=10,     help="dimensionless effective spontaneous curvature")
    parser.add_argument('--kade',       type=float, default=1.27e-19, help="area difference modulus")
    parser.add_argument('--A0',         type=float, default=140e-12, help="area of rbc")
    args = parser.parse_args()
    
    
    ref = trimesh.load(args.ref, process=False)
    triangles_ref = ref.triangles
    DA0i = trimesh.triangles.area(triangles=triangles_ref)
    Area_ref = ref.area
    RA = np.sqrt(Area_ref/4/np.pi)
    
    Nv = len(ref.vertices)
    triangle_list = ref.vertex_faces  # face indices that correspond to each vertex
    
    

    for i, file_ in enumerate(args.f):
        print(file_)

        # Mesh info:
        mesh = trimesh.load(file_, process=False)
        triangles_mesh = mesh.triangles
        DAi = trimesh.triangles.area(triangles=triangles_mesh)
        Area_mesh = mesh.area

        assert(Area_ref<=1.005*Area_mesh and Area_ref>=0.995*Area_mesh)
        assert( np.sum(np.abs(ref.faces - mesh.faces)) == 0 )  # check vertex ordering is the same! 


        # strain energy of membrane skeleton (MS), LWM
        ai, bi = compute_invariants(DAi, DA0i, triangles_mesh, triangles_ref)

        Estretch = (args.L_UNIT**2) * args.ka * np.sum( DA0i * compute_Earea(args.a3, args.a4, ai) )   # sum over all triangles
        Eshear   = (args.L_UNIT**2) * args.mu * np.sum( DA0i * compute_Eshear(args.b1, args.b2, ai, bi) )
        Ems      = Estretch + Eshear

        print("Ems (total): %e" % Ems)
        print("Estretch:    %e" % Estretch)
        print("Eshear:      %e" % Eshear)

