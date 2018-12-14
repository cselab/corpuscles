import re
import sys

import bpy
import mathutils

me = "oogl"

class Oogl(Exception):
    pass

def msg(fmt, *args, **kwargs):
    fmt = me + ": " + fmt
    print(fmt, *args, file=sys.stderr, **kwargs)

def parse(l):
    l = re.sub('#.*', "", l)
    l = l.strip()
    a = re.split('[ \\t]+', l)
    return a

def transform(path):
    try:
        return transform0(path)
    except FileNotFoundError:
        msg("canot open '%s'" % path)
        raise Oogl
    except IndexError:
        msg("fail to parse transform in '%s'" % path)
        raise Oogl

def transform0(path):
    ans = []
    with open(path) as f:
        f = open(path)
        lines = f.read().splitlines()
    n = len(lines)
    i = 0
    while i < n:
        a = parse(lines[i])
        i += 1
        if len(a) > 0 and a[0] == "transform" or \
           len(a) > 1 and a[1] == "transform":
            M = []
            for j in (0, 1, 2, 3):
                l = parse(lines[i]); i += 1
                try:
                    a = [float(e) for e in l]
                except ValueError:
                    msg("'%s'" % lines[i])
                    msg("not three floats '%s'" % l)
                    raise Oogl
                M.append(a)
            M = mathutils.Matrix(M)
            M.transpose()
            ans.append(M)

    if len(ans) == 1:
        ans = ans[0]
    else:
        ans = ans[0] * ans[1]
    return ans

def fov(path):
    with open(path) as f:
        lines = f.read().splitlines()
    n = len(lines)
    i = 0
    while i < n:
        a = parse(lines[i])
        i += 1
        if len(a)>1 and a[0] == "fov":
            return float(a[1])
    msg("cannot find fov in '%s'" % path)
    raise Oogl

def off(path):
    tris = []
    edges = []
    verts = []

    with open(path) as f:
        lines = f.read().splitlines()

    n = len(lines)
    i = 0
    while i < n:
        a = parse(lines[i])
        i += 1
        if a[0] == "OFF":
            break
    else:
        msg("cannot find OFF in '%s'" % path)
        raise Oogl

    while i < n:
        a = parse(lines[i])
        i += 1
        if len(a) == 3:
            nv, nt, ne = [int(e) for e in a]
            break
    else:
        msg("cannot find 'nv ne nt' in '%s'" % path)
        raise Oogl

    v = 0
    while i < n:
        a = parse(lines[i])
        i += 1
        if len(a) == 3:
            v += 1
            x, y, z = [float(e) for e in a]
            verts.append((x, y, z))
            if v == nv: break
    else:
        msg("fail to read verts in '%s'" % path)
        raise Oogl

    t = 0
    while i < n:
        a = parse(lines[i])
        i += 1
        if len(a) > 3:
            t += 1
            tri = [int(e) for e in a[1:]]
            tris.append(tri)
            if t == nt: break
    else:
        msg("fail to read faces in '%s'" % path)
        raise Oogl

    name = bpy.path.display_name_from_filepath(path)
    mesh = bpy.data.meshes.new(name=name)

    mesh.from_pydata(verts, edges, tris)
    mesh.validate()
    mesh.update()
    return mesh
