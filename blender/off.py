import os
import sys
import math

import bpy
import mathutils

sys.path.append(".")
import oogl

Iblender = "red.blend"
Camera = "data/stomatocyte/00001.camera"
Geometry = "data/stomatocyte/00001.geometry"
Oblender = "o.blend"
Opng = "o.png"

fov = oogl.fov(Camera)
print(fov)
fov *= math.pi/180
bpy.ops.wm.open_mainfile(filepath = Iblender)

mesh = oogl.off(Geometry)
cam = bpy.data.objects['camera']
cam.data.angle = fov
M = oogl.transform(Camera)
cam.matrix_world = M

cell = bpy.data.objects['cell']
cell.data = mesh
cell.active_material = bpy.data.materials['Cell']
M = oogl.transform(Geometry)
cell.matrix_world = M

bpy.ops.wm.save_as_mainfile(filepath = Oblender)

bpy.data.scenes['Scene'].render.filepath = Opng
# bpy.ops.render.render(write_still = True)
