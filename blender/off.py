import os
import sys
import math

import bpy
import mathutils

sys.path.append(".")
import oogl

Iblender = "preved.blend"
Camera = "data/00001.camera"
Geometry = "data/00001.geometry"
Oblender = "o.blend"
Opng = "o.png"
magic = 0.68885112

fov = oogl.fov(Camera)
print(fov)
fov /= magic
fov *= math.pi/180

bpy.ops.wm.open_mainfile(filepath = Iblender)

mesh = oogl.off(Geometry)
print(mesh)

cam = bpy.data.objects['camera']
cam.data.angle = fov
cam.matrix_world = oogl.transform(Camera)

cell = bpy.data.objects['cell']
cell.data = mesh
cell.active_material = bpy.data.materials['Cell']
M = oogl.transform(Geometry)
cell.matrix_world =  M
print(M)

bpy.ops.wm.save_as_mainfile(filepath = Oblender)

bpy.data.scenes['Scene'].render.filepath = Opng
#bpy.ops.render.render(write_still = True)

# blender --background --python off.py
