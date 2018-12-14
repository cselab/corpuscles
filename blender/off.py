import os
import sys
import math

import bpy
import mathutils

sys.path.append(".")
import oogl

def load(filepath):
    filepath = os.fsencode(filepath)
    file = open(filepath, 'r')
    first_line = file.readline().rstrip()
    use_colors = (first_line == 'COFF')
    colors = []

    line = file.readline()
    while line.isspace() or line[0]=='#':
        line = file.readline()

    vcount, fcount, ecount = [int(x) for x in line.split()]
    verts = []
    facets = []
    edges = []
    i=0;
    while i<vcount:
        line = file.readline()
        if line.isspace():
            continue
        try:
             bits = [float(x) for x in line.split()]
             px = bits[0]
             py = bits[1]
             pz = bits[2]
             if use_colors:
                 colors.append([float(bits[3]) / 255, float(bits[4]) / 255, float(bits[5]) / 255])

        except ValueError:
            i=i+1
            continue
        verts.append((px, py, pz))
        i=i+1

    i=0;
    while i<fcount:
        line = file.readline()
        if line.isspace():
            continue
        try:
            splitted  = line.split()
            ids   = list(map(int, splitted))
            if len(ids) > 3:
                facets.append(tuple(ids[1:]))
            elif len(ids) == 3:
                edges.append(tuple(ids[1:]))
        except ValueError:
            i=i+1
            continue
        i=i+1


    off_name = bpy.path.display_name_from_filepath(filepath)
    mesh = bpy.data.meshes.new(name=off_name)
    mesh.from_pydata(verts,edges,facets)

    mesh.validate()
    mesh.update()

    if use_colors:
        color_data = mesh.vertex_colors.new()
        for i, facet in enumerate(mesh.polygons):
            for j, vidx in enumerate(facet.vertices):
                color_data.data[3*i + j].color = colors[vidx]

    return mesh

Camera = "data/00001.camera"
Geometry = "data/00001.geometry"
Oblender = "o.blend"
Opng = "o.png"
magic = 0.68885112

fov = oogl.fov(Camera)
fov /= magic
fov *= math.pi/180

i = "preved.blend"
bpy.ops.wm.open_mainfile(filepath = i)

mesh = load("data/0.off")

cam = bpy.data.objects['camera']
cam.data.angle = fov
cam.matrix_world = oogl.transform(Camera)

cell = bpy.data.objects['cell']
cell.data = mesh
cell.active_material = bpy.data.materials['Cell']
cell.matrix_world =  oogl.transform(Geometry)

bpy.ops.wm.save_as_mainfile(filepath = Oblender)

bpy.data.scenes['Scene'].render.filepath = Opng
bpy.ops.render.render(write_still = True)

# blender --background --python off.py
