# state file generated using paraview version 5.7.0

# ----------------------------------------------------------------
# setup views used in the visualization
# ----------------------------------------------------------------

# trace generated using paraview version 5.7.0
#
# To ensure correct image size when batch processing, please search 
# for and uncomment the line `# renderView*.ViewSize = [*,*]`

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
def f(Input, Output):
    paraview.simple._DisableFirstRenderCameraReset()
    
    # Create a new 'Light'
    light1 = CreateLight()
    light1.Intensity = 20.0
    light1.Type = 'Positional'
    light1.Position = [4.816385193527325, -4.065172620623194, 15.0]
    light1.FocalPoint = [0.5, 0.5, 7.653097604113405e-17]
    light1.Radius = 5.0
    
    # Create a new 'Light'
    light2 = CreateLight()
    light2.Intensity = 2.0
    light2.Type = 'Positional'
    light2.Position = [-7.706738016320278, 0.012011812797779589, 2.8374185151280793]
    light2.Radius = 2.0
    
    # get the material library
    materialLibrary1 = GetMaterialLibrary()
    mf = "material/ospray_mats.json"
    materialLibrary1.LoadMaterials = mf
    
    # create light
    # create light
    # Create a new 'Render View'
    renderView1 = CreateView('RenderView')
    renderView1.ViewSize = [923, 748]
    renderView1.AxesGrid = 'GridAxes3DActor'
    renderView1.OrientationAxesVisibility = 0
    renderView1.CenterOfRotation = [0.0, 0.0, -0.46779561042785645]
    renderView1.UseLight = 0
    renderView1.StereoType = 'Crystal Eyes'
    renderView1.CameraPosition = [0.8935202003527616, -3.3479292047680493, 4.384846164524862]
    renderView1.CameraFocalPoint = [-6.745762604141978e-18, 5.2330764444252924e-17, -0.46779561042785645]
    renderView1.CameraViewUp = [-0.12487872936425458, 0.8057713631815907, 0.5789108853950238]
    renderView1.CameraFocalDisk = 1.0
    renderView1.CameraParallelScale = 7.091374032215009
    renderView1.Background = [0.32, 0.34, 0.43]
    renderView1.EnableRayTracing = 1
    renderView1.BackEnd = 'OSPRay pathtracer'
    renderView1.AmbientSamples = 1
    renderView1.SamplesPerPixel = 16
    renderView1.AdditionalLights = [light1, light2]
    renderView1.OSPRayMaterialLibrary = materialLibrary1
    
    SetActiveView(None)
    
    # ----------------------------------------------------------------
    # setup view layouts
    # ----------------------------------------------------------------
    
    # create new layout object 'Layout #1'
    layout1 = CreateLayout(name='Layout #1')
    layout1.AssignView(0, renderView1)
    
    # ----------------------------------------------------------------
    # restore active view
    SetActiveView(renderView1)
    # ----------------------------------------------------------------
    
    # ----------------------------------------------------------------
    # setup the data processing pipelines
    # ----------------------------------------------------------------
    
    # create a new 'Legacy VTK Reader'
    qvtk = LegacyVTKReader(FileNames=[Input])
    
    # create a new 'Generate Surface Normals'
    generateSurfaceNormals1 = GenerateSurfaceNormals(Input=qvtk)
    
    # create a new 'Plane'
    plane1 = Plane()
    plane1.Origin = [-5.0, -5.0, -1.0]
    plane1.Point1 = [5.0, -5.0, -1.0]
    plane1.Point2 = [-5.0, 5.0, -1.0]
    
    # ----------------------------------------------------------------
    # setup the visualization in view 'renderView1'
    # ----------------------------------------------------------------
    
    # show data from generateSurfaceNormals1
    generateSurfaceNormals1Display = Show(generateSurfaceNormals1, renderView1)
    
    # trace defaults for the display properties.
    generateSurfaceNormals1Display.Representation = 'Surface'
    generateSurfaceNormals1Display.ColorArrayName = ['POINTS', '']
    generateSurfaceNormals1Display.OSPRayScaleArray = 'color'
    generateSurfaceNormals1Display.OSPRayScaleFunction = 'PiecewiseFunction'
    generateSurfaceNormals1Display.OSPRayMaterial = 'aluminum'
    generateSurfaceNormals1Display.SelectOrientationVectors = 'None'
    generateSurfaceNormals1Display.ScaleFactor = 0.2
    generateSurfaceNormals1Display.SelectScaleArray = 'color'
    generateSurfaceNormals1Display.GlyphType = 'Arrow'
    generateSurfaceNormals1Display.GlyphTableIndexArray = 'color'
    generateSurfaceNormals1Display.GaussianRadius = 0.01
    generateSurfaceNormals1Display.SetScaleArray = ['POINTS', 'color']
    generateSurfaceNormals1Display.ScaleTransferFunction = 'PiecewiseFunction'
    generateSurfaceNormals1Display.OpacityArray = ['POINTS', 'color']
    generateSurfaceNormals1Display.OpacityTransferFunction = 'PiecewiseFunction'
    generateSurfaceNormals1Display.DataAxesGrid = 'GridAxesRepresentation'
    generateSurfaceNormals1Display.PolarAxes = 'PolarAxesRepresentation'
    
    # show data from plane1
    plane1Display = Show(plane1, renderView1)
    
    # trace defaults for the display properties.
    plane1Display.Representation = 'Surface'
    plane1Display.ColorArrayName = [None, '']
    plane1Display.OSPRayScaleArray = 'Normals'
    plane1Display.OSPRayScaleFunction = 'PiecewiseFunction'
    plane1Display.SelectOrientationVectors = 'None'
    plane1Display.ScaleFactor = 0.15000000000000002
    plane1Display.SelectScaleArray = 'None'
    plane1Display.GlyphType = 'Arrow'
    plane1Display.GlyphTableIndexArray = 'None'
    plane1Display.GaussianRadius = 0.0075
    plane1Display.SetScaleArray = ['POINTS', 'Normals']
    plane1Display.ScaleTransferFunction = 'PiecewiseFunction'
    plane1Display.OpacityArray = ['POINTS', 'Normals']
    plane1Display.OpacityTransferFunction = 'PiecewiseFunction'
    plane1Display.DataAxesGrid = 'GridAxesRepresentation'
    plane1Display.PolarAxes = 'PolarAxesRepresentation'
    
    # init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
    plane1Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]
    
    # init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
    plane1Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]
    
    # ----------------------------------------------------------------
    # finally, restore active source
    SetActiveSource(generateSurfaceNormals1)
    # ----------------------------------------------------------------
    SaveScreenshot(Output, renderView1, TransparentBackground = 0)
