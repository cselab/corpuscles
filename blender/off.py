import os
import bpy

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

i = "preved.blend"
o = "o.blend"
bpy.ops.wm.open_mainfile(filepath = i)

mesh = load("0.off")

bpy.data.objects[0].data = mesh
bpy.data.objects[0].active_material = bpy.data.materials[0]

bpy.data.scenes['Scene'].render.filepath = 'o.png'
bpy.ops.render.render(write_still = True)

bpy.ops.wm.save_as_mainfile(filepath = o)
