import math
import sys
import numpy as np
import trimesh
import argparse
import matplotlib.pyplot as plt

from color_by_inimesh import export_colored_faces


def vec_cross(a, b):
    X=0; Y=1; Z=2
    c = np.empty(np.shape(a))
    c[X] = a[Y] * b[Z] - b[Y] * a[Z];
    c[Y] = b[X] * a[Z] - a[X] * b[Z];
    c[Z] = a[X] * b[Y] - b[X] * a[Y];
    return c;


def single_angle_between(v1, v2):
    x = np.dot(v1, v2)
    n = vec_cross(v1, v2)
    y = np.sqrt(np.dot(n, n))
    return math.atan2(y, x)


def angle_between(vec1, vec2):
    angles = np.zeros(len(vec1))
    for i, v1, v2 in zip(range(len(vec1)), vec1, vec2):
        angles[i] = single_angle_between(v1, v2)
    return angles


def single_compute_magnitude(v):
    return np.sqrt( np.dot(v, v) )


def compute_magnitude(vec):
    mag = np.zeros(len(vec))
    for i, v in enumerate(vec):
        mag[i] = single_compute_magnitude(v)
    return mag


def tri_normal(a, b, c):
    """
    from:
    tri_normal(const real a[3], const real b[3], const real c[3],  /**/ real e[3])
    {
        real u[3], v[3], n[3];
        vec_minus(b, a, u);
        vec_minus(c, a, v);
        vec_cross(u, v, n);
        vec_norm(n, e);
    }
 
    """
    u = b - a
    v = c - a
    n = vec_cross(u, v)
    InI = single_compute_magnitude(n)
    return (n/InI)



def kantor(thetas):
    """
    Eq. 17 in Bian2020
    len(thetas) = N_edges
    """
    import math
    c = math.sqrt(3.0) / 3.0
    E = 16.0*np.pi/3.0  +  2.0 * c * np.sum(1.0-np.cos(thetas))
    return E



def dih_angle_sup(a, b, c, d):
    """
    Based on `dih_angle_sup':
     repo: https://github.com/cselab/corpuscles
     file: corpuscles/lib/f/juelicher_xin.c
     date: 16 Nov 2020

 27 dih_angle_sup(const real a[3], const real b[3], const real c[3],
 28               const real d[3])
 29 {
 30     /*supplementary angle of dihedral */
 31     real x, y, ang, n[3], k[3], nk[3], e[3];
 32
 33     tri_normal(b, c, a, n);
 34     tri_normal(c, b, d, k);
 35     x = vec_dot(n, k);
 36     vec_cross(n, k, nk);
 37     y = vec_abs(nk);
 38     ang = atan2(y, x);
 39     vec_minus(c, b, e);
 40     if (vec_dot(e, nk) < 0)
 41         ang = -ang;
 42     return ang;
 43 }
    """

    n = tri_normal(b, c, a)
    k = tri_normal(c, b, d)
    x = np.dot(n, k)
    nk = vec_cross(n, k)
    y = single_compute_magnitude(nk)
    ang = math.atan2(y, x)
    e = c - b

    if (np.dot(e, nk) < 0):
        ang = -ang

    return ang
 


def compute_juelicher(mesh):
    """
    bc: common edge
    triangle 0: (b, c, a)
    triangle 1: (c, b, d)
            c ---- d
           / \    /
          /   \  /
         a ---- b 
    """

    Nv = len(mesh.vertices)
    Hk = np.zeros(Nv)
    Ak = np.zeros(Nv)

    vertices = mesh.vertices


    # 1/2: Area per vertex
    At = mesh.area_faces
    for k, (iv0, iv1, iv2) in enumerate(mesh.faces):
        area = At[k]
        Ak[iv0] += area
        Ak[iv1] += area
        Ak[iv2] += area
    Ak = Ak / 3.0


    # 2/2: Local curvature per vertex
    adjacent_faces = mesh.face_adjacency   # Pairs of faces which share an edge
    faces = mesh.faces
    for k, (iv0, iv1) in enumerate(mesh.edges_unique):

        # indices of adjacent faces
        if0 = adjacent_faces[k,0]
        if1 = adjacent_faces[k,1]
        face0 = faces[if0]
        face1 = faces[if1]

        # vertex indices for triangles in dihedral
        m1 = iv0
        m2 = iv1
        for _iv in face0:
            if (_iv != m1) and (_iv != m2):
                ia = _iv
        for _iv in face1:
            if (_iv != m1) and (_iv != m2):
                idd = _iv

        # face0
        if   (face0 == np.asarray([ia, m1, m2])).all(): # abc
            ib=m1; ic=m2
        elif (face0 == np.asarray([ia, m2, m1])).all():
            ib=m2; ic=m1
        elif (face0 == np.asarray([m1, ia, m2])).all(): # cab
            ib=m2; ic=m1
        elif (face0 == np.asarray([m2, ia, m1])).all():
            ib=m1; ic=m2
        elif (face0 == np.asarray([m1, m2, ia])).all(): # bca
            ib=m1; ic=m2
        elif (face0 == np.asarray([m2, m1, ia])).all():
            ib=m2; ic=m1
        else:
            print("Error!")
            sys.exit()
            assert(0)

        # face1
        if   (face1 == np.asarray([idd, m1, m2])).all(): # dcb
            ib_=m2; ic_=m1
        elif (face1 == np.asarray([idd, m2, m1])).all():
            ib_=m1; ic_=m2
        elif (face1 == np.asarray([m1, idd, m2])).all(): # bdc
            ib_=m1; ic_=m2
        elif (face1 == np.asarray([m2, idd, m1])).all():
            ib_=m2; ic_=m1
        elif (face1 == np.asarray([m1, m2, idd])).all(): # cbd
            ib_=m2; ic_=m1
        elif (face1 == np.asarray([m2, m1, idd])).all():
            ib_=m1; ic_=m2
        else:
            print("Error!")
            sys.exit()
            assert(0)


        if ib!=ib_ or ic!=ic_:
            assert(0)

            _t_ = ia
            ia = idd
            idd = _t_

            # face0
            if   (face0 == np.asarray([ia, m1, m2])).all(): # abc
                ib=m1; ic=m2
            elif (face0 == np.asarray([ia, m2, m1])).all():
                ib=m2; ic=m1
            elif (face0 == np.asarray([m1, ia, m2])).all(): # cab
                ib=m2; ic=m1
            elif (face0 == np.asarray([m2, ia, m1])).all():
                ib=m1; ic=m2
            elif (face0 == np.asarray([m1, m2, ia])).all(): # bca
                ib=m1; ic=m2
            elif (face0 == np.asarray([m2, m1, ia])).all():
                ib=m2; ic=m1
            else:
                print("Error!")
                sys.exit()
                assert(0)

            # face1
            if   (face1 == np.asarray([idd, m1, m2])).all(): # dcb
                ib_=m2; ic_=m1
            elif (face1 == np.asarray([idd, m2, m1])).all():
                ib_=m1; ic_=m2
            elif (face1 == np.asarray([m1, idd, m2])).all(): # bdc
                ib_=m1; ic_=m2
            elif (face1 == np.asarray([m2, idd, m1])).all():
                ib_=m2; ic_=m1
            elif (face1 == np.asarray([m1, m2, idd])).all(): # cbd
                ib_=m2; ic_=m1
            elif (face1 == np.asarray([m2, m1, idd])).all():
                ib_=m1; ic_=m2
            else:
                print("Error!")
                sys.exit()
                assert(0)

        if ib!=ib_ or ic!=ic_:
            assert(0)


        # vertices of triangles
        a = vertices[ia]
        b = vertices[ib]
        c = vertices[ic]
        d = vertices[idd]

        # angle between triangles
        thj = dih_angle_sup(a, b, c, d)

        # length of common edge between triangles
        e = c - b
        lk = single_compute_magnitude(e)

        # edge curvature
        cur = lk * thj

        # accumulate curvature on vertices
        Hk[iv0] += cur
        Hk[iv1] += cur

    Hk = 0.25 * Hk / Ak

    return Hk, Ak




if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('--f', type=str, required=True, nargs='+', help="mesh files")

    parser.add_argument('--L_UNIT',     type=float, default=3.33779e-6, help="length unit, m")
    parser.add_argument('--kb',         type=float, default=2.e-19, help="bending modulus")
    parser.add_argument('--m0bar',      type=float, default=10,     help="dimensionless effective spontaneous curvature")
    parser.add_argument('--kade',       type=float, default=1.27e-19, help="area difference modulus")
    parser.add_argument('--A0',         type=float, default=140e-12, help="area of rbc")
    args = parser.parse_args()


    for i, file_ in enumerate(args.f):
        print(file_)

        mesh = trimesh.load(file_, process=False)
        Nv = len(mesh.vertices)


        # Minimum energy - Kantor
        Eb_kantor = args.kb * kantor(mesh.face_adjacency_angles)
        print("Eb_local (Kantor):", Eb_kantor)


        # local curvature and area (per vertex)
        Hk, Ak = compute_juelicher(mesh)
        M1 = np.sum(Hk      * Ak)  *  args.L_UNIT
        M2 = np.sum(Hk * Hk * Ak)
        M12 = M1*M1


        # Minimum energy - Juelicher
        Eb_local = 2.0 * args.kb * M2
        print("Eb_local (Juelicher):", Eb_local)


        # ADE energy. Ref: X.Bian, CMEA, 2020
        RA = np.sqrt(args.A0/4.0/np.pi)
        DA0D = 2.0 * args.m0bar * RA
        len_theta = 4. * Hk * Ak * args.L_UNIT
        len_theta_tot = np.sum( len_theta )
        scurv = (len_theta_tot / 2 - DA0D) / args.A0;
        Eb_ade = np.pi * args.kade * args.A0 * scurv * scurv / 2.0;
        print("Eb_ade (Juelicher):", Eb_ade)

