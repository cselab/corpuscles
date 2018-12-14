import bpy
import types
import dumper
from pprint import pprint
import json
import collections.abc

i = "preved.blend"
bpy.ops.wm.open_mainfile(filepath = i)

data = bpy

def iterable(x):
    return isinstance(x, collections.abc.Container) and iterable0(x)

def iterable0(x):
    try:
        iter(x)
        return True
    except TypeError:
        return False


def dirp(x):
    try:
        dir(x)
        return True
    except TypeError:
        return False

M = set()

def dump(a, lvl = 0):
    try:
        if a in M: return
        M.add(a)
    except:
        pass
    if isinstance(a, str) or isinstance(a, int) or isinstance(a, str):
        pass
    elif iterable(a):
        print("iterable", len(a))
        for e in a:
            dump(e, lvl + 1)
    elif dirp(a):
        for key in dir(a):
            if key.startswith('_'): continue
            try:
                val = getattr(a, key)
                print("%02d : %-20s : %-40s : %-30s" %
                      (lvl, str(key), str(val), str(type(val))))
                if lvl < maxlevel:
                    dump(val, lvl + 1)
            except:
                pass
            
maxlevel = 0
t = bpy.data.objects[2]
dump(t)

# blender --background --python dump.py
