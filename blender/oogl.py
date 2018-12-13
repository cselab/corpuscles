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
    l = l.strip(' ')
    a = re.split('[ \t]+', l)
    return a

def transform(path):
    try:
        return transform0(path)
    except FileNotFoundError:
        msg("canot open '%s'" % path)
        raise Oogl
    except ValueError:
        msg("fail to parse transform in '%s'" % path)
        raise Oogl
    except IndexError:
        msg("fail to parse transform in '%s'" % path)
        raise Oogl 

def transform0(path):
    M = []
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
                a = [float(e) for e in parse(lines[i])]
                i += 1
                M.append(tuple(a))
            break
    f.close()
    if not M:
        msg("no transform in '%s'" % path)
        raise Oogl
    M = mathutils.Matrix(M)
    M.transpose()
    return M

