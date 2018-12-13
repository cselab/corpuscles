import bpy
import types
import dumper
from pprint import pprint

i = "preved.blend"
bpy.ops.wm.open_mainfile(filepath = i)

data = bpy

def iterable(x):
    try:
        iter(x)
        return True
    except TypeError:
        return False

def dictp(x):
    try:
        vars(x)
        return True
    except TypeError:
        return False

M = set()
    
def dump(a):
    i = id(a)
    if i in M: return
    M.add(i)
    if isinstance(a, str):
        print("a = " + str(a))
    elif iterable(a):
        for e in a:
            dump(e)
    elif dictp(a):
        for key in vars(a):
            if key.startswith('_'): continue
            val = getattr(a, key)
            dump(val)
    else:
        print("%s %s" % (str(a), str(type(a))))

dump(bpy)


# blender --background --python dump.py
