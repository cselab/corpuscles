import re
import sys
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
    M = []
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
                for j in (0, 1, 2, 3):
                    l = parse(lines[i]); i += 1
                    try:
                        a = [float(e) for e in l]
                    except ValueError:
                        msg("'%s'" % lines[i])
                        msg("not three floats '%s'" % l)
                        raise Oogl
                    M.append(tuple(a))
                break
    if not M:
        msg("no transform in '%s'" % path)
        raise Oogl
    M = mathutils.Matrix(M)
    M.transpose()
    return M

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
    off_name = bpy.path.display_name_from_filepath(path)
    mesh = bpy.data.meshes.new(name=off_name)
    mesh.from_pydata(verts,edges,facets)

    mesh.validate()
    mesh.update()
    
    return mesh
