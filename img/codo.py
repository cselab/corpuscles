from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()
SamplesPerPixel = 50

mtl = GetMaterialLibrary()
mtl.LoadMaterials = "codo.json"

light = CreateLight()
light.Coords = 'Ambient'

render = CreateView('RenderView')
render.ViewSize = [1053, 836]
render.AxesGrid = 'GridAxes3DActor'
render.OrientationAxesVisibility = 0
render.CenterOfRotation = [-1.9761497265236017e-06, 9.267043574512357e-06, -0.11117156609963116]
render.UseLight = 0
render.StereoType = 'Crystal Eyes'
render.CameraPosition = [-1.9761497265236017e-06, 9.267043574512357e-06, 6.816658013599184]
render.CameraFocalPoint = [-1.9761497265236017e-06, 9.267043574512357e-06, -0.11117156609963116]
render.CameraFocalDisk = 1.0
render.CameraParallelScale = 1.793054236450645
render.EnableRayTracing = 1
render.BackEnd = 'OSPRay pathtracer'
render.SamplesPerPixel = SamplesPerPixel
render.AdditionalLights = light
render.OSPRayMaterialLibrary = mtl
SetActiveView(None)

layout = CreateLayout(name='Layout #1')
layout.AssignView(0, render)
layout.SetSize(1053, 836)

SetActiveView(render)
vtk = LegacyVTKReader(registrationName='codo.vtk', FileNames=['codo.vtk'])

# create a new 'Plane'
plane = Plane(registrationName='Plane')
plane.Origin = [-2.0, -2.0, -0.5]
plane.Point1 = [2.0, -2.0, -0.5]
plane.Point2 = [-2.0, 2.0, -0.5]

# create a new 'Generate Surface Normals'
generateSurfaceNormals1 = GenerateSurfaceNormals(registrationName='GenerateSurfaceNormals1', Input=vtk)

# ----------------------------------------------------------------
# setup the visualization in view 'render'
# ----------------------------------------------------------------

# show data from plane
planeDisplay = Show(plane, render, 'GeometryRepresentation')

# trace defaults for the display properties.
planeDisplay.Representation = 'Surface'
planeDisplay.ColorArrayName = [None, '']
planeDisplay.SelectTCoordArray = 'TextureCoordinates'
planeDisplay.SelectNormalArray = 'Normals'
planeDisplay.SelectTangentArray = 'None'
planeDisplay.OSPRayScaleArray = 'Normals'
planeDisplay.OSPRayScaleFunction = 'PiecewiseFunction'
planeDisplay.SelectOrientationVectors = 'None'
planeDisplay.ScaleFactor = 0.1
planeDisplay.SelectScaleArray = 'None'
planeDisplay.GlyphType = 'Arrow'
planeDisplay.GlyphTableIndexArray = 'None'
planeDisplay.GaussianRadius = 0.005
planeDisplay.SetScaleArray = ['POINTS', 'Normals']
planeDisplay.ScaleTransferFunction = 'PiecewiseFunction'
planeDisplay.OpacityArray = ['POINTS', 'Normals']
planeDisplay.OpacityTransferFunction = 'PiecewiseFunction'
planeDisplay.DataAxesGrid = 'GridAxesRepresentation'
planeDisplay.PolarAxes = 'PolarAxesRepresentation'
planeDisplay.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]
planeDisplay.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]
generateSurfaceNormals1Display = Show(generateSurfaceNormals1, render, 'GeometryRepresentation')
generateSurfaceNormals1Display.Representation = 'Surface'
generateSurfaceNormals1Display.ColorArrayName = [None, '']
generateSurfaceNormals1Display.SelectTCoordArray = 'None'
generateSurfaceNormals1Display.SelectNormalArray = 'Normals'
generateSurfaceNormals1Display.SelectTangentArray = 'None'
generateSurfaceNormals1Display.OSPRayScaleArray = 'Normals'
generateSurfaceNormals1Display.OSPRayScaleFunction = 'PiecewiseFunction'
generateSurfaceNormals1Display.OSPRayMaterial = 'codo'
generateSurfaceNormals1Display.SelectOrientationVectors = 'None'
generateSurfaceNormals1Display.ScaleFactor = 0.2485215120504413
generateSurfaceNormals1Display.SelectScaleArray = 'None'
generateSurfaceNormals1Display.GlyphType = 'Arrow'
generateSurfaceNormals1Display.GlyphTableIndexArray = 'None'
generateSurfaceNormals1Display.GaussianRadius = 0.012426075602522065
generateSurfaceNormals1Display.SetScaleArray = ['POINTS', 'Normals']
generateSurfaceNormals1Display.ScaleTransferFunction = 'PiecewiseFunction'
generateSurfaceNormals1Display.OpacityArray = ['POINTS', 'Normals']
generateSurfaceNormals1Display.OpacityTransferFunction = 'PiecewiseFunction'
generateSurfaceNormals1Display.DataAxesGrid = 'GridAxesRepresentation'
generateSurfaceNormals1Display.PolarAxes = 'PolarAxesRepresentation'
generateSurfaceNormals1Display.ScaleTransferFunction.Points = [-0.9985520839691162, 0.0, 0.5, 0.0, 0.9985434412956238, 1.0, 0.5, 0.0]
generateSurfaceNormals1Display.OpacityTransferFunction.Points = [-0.9985520839691162, 0.0, 0.5, 0.0, 0.9985434412956238, 1.0, 0.5, 0.0]
SetActiveSource(generateSurfaceNormals1)
SaveScreenshot("a.png")
