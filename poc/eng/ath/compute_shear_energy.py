import numpy as np
import trimesh
import argparse


def unit_vector(vector):
    """ Returns the unit vector of the vector.  """
    return vector / np.linalg.norm(vector)


def angle_between(vec1, vec2):
    """ Returns the angle in radians between vectors 'v1' and 'v2'::
    """
    angles = np.zeros(len(vec1))
    for i, v1, v2 in zip(range(len(vec1)), vec1, vec2):
        v1_u = unit_vector(v1)
        v2_u = unit_vector(v2)
        angles[i] = np.arccos(np.clip(np.dot(v1_u, v2_u), -1.0, 1.0))
    return angles


def compute_magnitude(vec):
    mag = np.zeros(len(vec))
    for i, v in enumerate(vec):
        mag[i] = np.sqrt( v.dot(v) )
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



parser = argparse.ArgumentParser()
parser.add_argument('--files',  type=str, required=True, nargs='+', help="rbc mesh files")
parser.add_argument('--ref',    type=str, required=True, help="stress-free mesh file")
parser.add_argument('--L_UNIT', type=float, required=True, help="length unit, m")

parser.add_argument('--mu',     type=float, default=2.5e-6, help="shear modulus")
parser.add_argument('--b1',     type=float, default=0.7,    help="non-linear shear coefficients")
parser.add_argument('--b2',     type=float, default=0.75,   help="non-linear shear coefficients")
parser.add_argument('--ka',     type=float, default=5.0e-6, help="stretch modulus")
parser.add_argument('--a3',     type=float, default=-2,     help="non-linear stretch coefficients")
parser.add_argument('--a4',     type=float, default=8,      help="non-linear stretch coefficients")
args = parser.parse_args()


ref = trimesh.load(args.ref, process=False)
triangles_ref = ref.triangles
DA0i = trimesh.triangles.area(triangles=triangles_ref)
Area_ref = ref.area
RA = np.sqrt(Area_ref/4/np.pi)

Nv = len(ref.vertices)
triangle_list = ref.vertex_faces  # face indices that correspond to each vertex



for i, file_ in enumerate(args.files):
    print(file_)

    # Mesh info:
    mesh = trimesh.load(file_, process=False)
    triangles_mesh = mesh.triangles
    DAi = trimesh.triangles.area(triangles=triangles_mesh)
    Area_mesh = mesh.area

    assert(Area_ref<=1.005*Area_mesh and Area_ref>=0.995*Area_mesh)


    # strain energy
    ai, bi = compute_invariants(DAi, DA0i, triangles_mesh, triangles_ref)

    ai2 = ai  * ai
    ai3 = ai2 * ai
    ai4 = ai2 * ai2
    bi2 = bi  * bi

    Estretch = (args.L_UNIT**2) * np.sum( DA0i * ( 0.5*args.ka * (ai2 + args.a3*ai3   + args.a4*ai4) ))   # sum over all triangles
    Eshear   = (args.L_UNIT**2) * np.sum( DA0i * (     args.mu * (bi  + args.b1*ai*bi + args.b2*bi2) ))
    Ems      = Estretch + Eshear

    print(Eshear, Estretch, Ems)


