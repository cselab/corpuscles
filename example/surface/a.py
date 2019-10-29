# state file generated using paraview version 5.5.1

# ----------------------------------------------------------------
# setup views used in the visualization
# ----------------------------------------------------------------

# trace generated using paraview version 5.5.1

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get the material library
materialLibrary1 = GetMaterialLibrary()

# Create a new 'Render View'
renderView1 = CreateView('RenderView')
renderView1.ViewSize = [1081, 861]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.StereoType = 0
renderView1.CameraPosition = [-0.06639579949199569, -3.497418118038352, 2.942109625083673]
renderView1.CameraViewUp = [0.9473537951210076, -0.21645985996474684, -0.2359362538777504]
renderView1.CameraParallelScale = 1.1830140069454795
renderView1.CameraParallelProjection = 1
renderView1.Background = [0.3686274509803922, 0.3686274509803922, 0.3686274509803922]
renderView1.OSPRayMaterialLibrary = materialLibrary1

# init the 'GridAxes3DActor' selected for 'AxesGrid'
renderView1.AxesGrid.XTitleFontFile = ''
renderView1.AxesGrid.YTitleFontFile = ''
renderView1.AxesGrid.ZTitleFontFile = ''
renderView1.AxesGrid.XLabelFontFile = ''
renderView1.AxesGrid.YLabelFontFile = ''
renderView1.AxesGrid.ZLabelFontFile = ''

# ----------------------------------------------------------------
# restore active view
SetActiveView(renderView1)
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# setup the data processing pipelines
# ----------------------------------------------------------------

# create a new 'Legacy VTK Reader'
avtk = LegacyVTKReader(FileNames=['/u/co/example/surface/a.vtk'])

# create a new 'Legacy VTK Reader'
bvtk = LegacyVTKReader(FileNames=['/u/co/example/surface/b.vtk'])

# create a new 'Calculator'
edg = Calculator(Input=bvtk)
edg.Function = ''

# create a new 'Cell Data to Point Data'
cellDatatoPointData1 = CellDatatoPointData(Input=edg)

# create a new 'Gaussian Resampling'
gaussianResampling1 = GaussianResampling(Input=cellDatatoPointData1)
gaussianResampling1.ResampleField = ['POINTS', 'c']
gaussianResampling1.ExtenttoResample = [-1.0, 1.0, -1.0, 1.0, -1.0, 1.0]
gaussianResampling1.GaussianSplatRadius = 0.01
gaussianResampling1.ScaleSplats = 0
gaussianResampling1.EllipticalSplats = 0
gaussianResampling1.FillVolumeBoundary = 0
gaussianResampling1.SplatAccumulationMode = 'Sum'

# create a new 'Point Data to Cell Data'
pointDatatoCellData1 = PointDatatoCellData(Input=cellDatatoPointData1)

# create a new 'Cell Data to Point Data'
cellDatatoPointData2 = CellDatatoPointData(Input=pointDatatoCellData1)

# ----------------------------------------------------------------
# setup the visualization in view 'renderView1'
# ----------------------------------------------------------------

# show data from cellDatatoPointData1
cellDatatoPointData1Display = Show(cellDatatoPointData1, renderView1)

# get color transfer function/color map for 'c'
cLUT = GetColorTransferFunction('c')
cLUT.RGBPoints = [0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]
cLUT.ScalarRangeInitialized = 1.0

# get opacity transfer function/opacity map for 'c'
cPWF = GetOpacityTransferFunction('c')
cPWF.Points = [0.0, 0.0, 0.5, 0.0, 1.0, 0.01, 0.5, 0.0]
cPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
cellDatatoPointData1Display.Representation = 'Volume'
cellDatatoPointData1Display.ColorArrayName = ['POINTS', 'c']
cellDatatoPointData1Display.LookupTable = cLUT
cellDatatoPointData1Display.OSPRayScaleFunction = 'PiecewiseFunction'
cellDatatoPointData1Display.SelectOrientationVectors = 'None'
cellDatatoPointData1Display.ScaleFactor = 0.202020202020202
cellDatatoPointData1Display.SelectScaleArray = 'c'
cellDatatoPointData1Display.GlyphType = 'Arrow'
cellDatatoPointData1Display.GlyphTableIndexArray = 'c'
cellDatatoPointData1Display.GaussianRadius = 0.0101010101010101
cellDatatoPointData1Display.SetScaleArray = [None, '']
cellDatatoPointData1Display.ScaleTransferFunction = 'PiecewiseFunction'
cellDatatoPointData1Display.OpacityArray = [None, '']
cellDatatoPointData1Display.OpacityTransferFunction = 'PiecewiseFunction'
cellDatatoPointData1Display.DataAxesGrid = 'GridAxesRepresentation'
cellDatatoPointData1Display.SelectionCellLabelFontFile = ''
cellDatatoPointData1Display.SelectionPointLabelFontFile = ''
cellDatatoPointData1Display.PolarAxes = 'PolarAxesRepresentation'
cellDatatoPointData1Display.ScalarOpacityUnitDistance = 0.0349909254054319
cellDatatoPointData1Display.ScalarOpacityFunction = cPWF
cellDatatoPointData1Display.IsosurfaceValues = [0.5]
cellDatatoPointData1Display.Slice = 50

# init the 'GridAxesRepresentation' selected for 'DataAxesGrid'
cellDatatoPointData1Display.DataAxesGrid.XTitleFontFile = ''
cellDatatoPointData1Display.DataAxesGrid.YTitleFontFile = ''
cellDatatoPointData1Display.DataAxesGrid.ZTitleFontFile = ''
cellDatatoPointData1Display.DataAxesGrid.XLabelFontFile = ''
cellDatatoPointData1Display.DataAxesGrid.YLabelFontFile = ''
cellDatatoPointData1Display.DataAxesGrid.ZLabelFontFile = ''

# init the 'PolarAxesRepresentation' selected for 'PolarAxes'
cellDatatoPointData1Display.PolarAxes.PolarAxisTitleFontFile = ''
cellDatatoPointData1Display.PolarAxes.PolarAxisLabelFontFile = ''
cellDatatoPointData1Display.PolarAxes.LastRadialAxisTextFontFile = ''
cellDatatoPointData1Display.PolarAxes.SecondaryRadialAxesTextFontFile = ''

# show data from cellDatatoPointData2
cellDatatoPointData2Display = Show(cellDatatoPointData2, renderView1)

# get separate color transfer function/color map for 'c'
separate_cellDatatoPointData2Display_cLUT = GetColorTransferFunction('c', cellDatatoPointData2Display, separate=True)
separate_cellDatatoPointData2Display_cLUT.RGBPoints = [0.0, 1.0, 1.0, 1.0, 0.18421052396297455, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0]
separate_cellDatatoPointData2Display_cLUT.ColorSpace = 'RGB'
separate_cellDatatoPointData2Display_cLUT.NanColor = [1.0, 0.0, 0.0]
separate_cellDatatoPointData2Display_cLUT.ScalarRangeInitialized = 1.0

# get separate opacity transfer function/opacity map for 'c'
separate_cellDatatoPointData2Display_cPWF = GetOpacityTransferFunction('c', cellDatatoPointData2Display, separate=True)
separate_cellDatatoPointData2Display_cPWF.Points = [0.0, 0.0, 0.5, 0.0, 0.19298246502876282, 0.125, 0.5, 0.0, 0.4969879388809204, 0.11764705926179886, 0.5, 0.0, 1.0, 0.0, 0.5, 0.0]
separate_cellDatatoPointData2Display_cPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
cellDatatoPointData2Display.Representation = 'Volume'
cellDatatoPointData2Display.ColorArrayName = ['POINTS', 'c']
cellDatatoPointData2Display.LookupTable = separate_cellDatatoPointData2Display_cLUT
cellDatatoPointData2Display.OSPRayScaleFunction = 'PiecewiseFunction'
cellDatatoPointData2Display.SelectOrientationVectors = 'None'
cellDatatoPointData2Display.ScaleFactor = 0.202020202020202
cellDatatoPointData2Display.SelectScaleArray = 'c'
cellDatatoPointData2Display.GlyphType = 'Arrow'
cellDatatoPointData2Display.GlyphTableIndexArray = 'c'
cellDatatoPointData2Display.GaussianRadius = 0.0101010101010101
cellDatatoPointData2Display.SetScaleArray = [None, '']
cellDatatoPointData2Display.ScaleTransferFunction = 'PiecewiseFunction'
cellDatatoPointData2Display.OpacityArray = [None, '']
cellDatatoPointData2Display.OpacityTransferFunction = 'PiecewiseFunction'
cellDatatoPointData2Display.DataAxesGrid = 'GridAxesRepresentation'
cellDatatoPointData2Display.SelectionCellLabelFontFile = ''
cellDatatoPointData2Display.SelectionPointLabelFontFile = ''
cellDatatoPointData2Display.PolarAxes = 'PolarAxesRepresentation'
cellDatatoPointData2Display.ScalarOpacityUnitDistance = 0.0349909254054319
cellDatatoPointData2Display.ScalarOpacityFunction = separate_cellDatatoPointData2Display_cPWF
cellDatatoPointData2Display.IsosurfaceValues = [0.5]
cellDatatoPointData2Display.Slice = 50

# init the 'GridAxesRepresentation' selected for 'DataAxesGrid'
cellDatatoPointData2Display.DataAxesGrid.XTitleFontFile = ''
cellDatatoPointData2Display.DataAxesGrid.YTitleFontFile = ''
cellDatatoPointData2Display.DataAxesGrid.ZTitleFontFile = ''
cellDatatoPointData2Display.DataAxesGrid.XLabelFontFile = ''
cellDatatoPointData2Display.DataAxesGrid.YLabelFontFile = ''
cellDatatoPointData2Display.DataAxesGrid.ZLabelFontFile = ''

# init the 'PolarAxesRepresentation' selected for 'PolarAxes'
cellDatatoPointData2Display.PolarAxes.PolarAxisTitleFontFile = ''
cellDatatoPointData2Display.PolarAxes.PolarAxisLabelFontFile = ''
cellDatatoPointData2Display.PolarAxes.LastRadialAxisTextFontFile = ''
cellDatatoPointData2Display.PolarAxes.SecondaryRadialAxesTextFontFile = ''

# set separate color map
cellDatatoPointData2Display.UseSeparateColorMap = True

# setup the color legend parameters for each legend in this view

# get color legend/bar for cLUT in view renderView1
cLUTColorBar = GetScalarBar(cLUT, renderView1)
cLUTColorBar.WindowLocation = 'UpperRightCorner'
cLUTColorBar.Title = 'c'
cLUTColorBar.ComponentTitle = ''
cLUTColorBar.TitleFontFile = ''
cLUTColorBar.LabelFontFile = ''

# set color bar visibility
cLUTColorBar.Visibility = 1

# get color legend/bar for separate_cellDatatoPointData2Display_cLUT in view renderView1
separate_cellDatatoPointData2Display_cLUTColorBar = GetScalarBar(separate_cellDatatoPointData2Display_cLUT, renderView1)
separate_cellDatatoPointData2Display_cLUTColorBar.Title = 'c'
separate_cellDatatoPointData2Display_cLUTColorBar.ComponentTitle = ''
separate_cellDatatoPointData2Display_cLUTColorBar.TitleFontFile = ''
separate_cellDatatoPointData2Display_cLUTColorBar.LabelFontFile = ''

# set color bar visibility
separate_cellDatatoPointData2Display_cLUTColorBar.Visibility = 1

# show color legend
cellDatatoPointData1Display.SetScalarBarVisibility(renderView1, True)

# show color legend
cellDatatoPointData2Display.SetScalarBarVisibility(renderView1, True)

# ----------------------------------------------------------------
# setup color maps and opacity mapes used in the visualization
# note: the Get..() functions create a new object, if needed
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# finally, restore active source
SetActiveSource(cellDatatoPointData1)
# ----------------------------------------------------------------