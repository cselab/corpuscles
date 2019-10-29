#!/usr/bin/env pvbatch

from paraview.simple import *

from glob import glob
import sys
import re
import math
import os
import numpy as np

def Log(s):
    s += "\n"
    o = sys.stderr
    o.write(s)
    o.flush()

# natural sort
def natkey(s, _nsre=re.compile('([0-9]+)')):
    return [int(text) if text.isdigit() else text.lower()
            for text in re.split(_nsre, s)]

def natsorted(v):
    return sorted(v, key=natkey)
# Returns sorted list of files in base by pattern pre_*.xmf

# Sets time of datasets to step i
def SetTime(i):
    global vft, vt
    for j in range(len(vft)):
        s = vft[j]
        s.ForcedTime = vt[j][i]
        s.UpdatePipeline()

# Returns bounding box of object o
def GetBox(o):
    o.UpdatePipeline()
    di = o.GetDataInformation()
    lim = di.DataInformation.GetBounds()
    lim0 = np.array(lim[::2])
    lim1 = np.array(lim[1::2])
    return lim0, lim1

av = sys.argv
if len(av) < 2 or av[1] == '-h':
    sys.stderr.write('''usage: {:} [sm_*.vtk]
Plots isosurface.
# Output:
# a_*.png in current folder
'''.format(av[0]))
    exit(1)

def CheckFlag(name):
    if name in av:
        av.remove(name)
        return True
    return False

draft = CheckFlag('-draft')
fine = CheckFlag('-fine')
fine2 = CheckFlag('-fine2')

# vf input
ff = natsorted(av[1:])
# vf basename
ffb = list(map(os.path.basename, ff))
# vf dirname
ffd = list(map(os.path.dirname, ff))
# steps
ss = [int(re.findall("_([0-9]*)", fb)[0]) for fb in ffb]

# output pattern (:0 substituted by frame number)
bo = "a_{:}.png"

#####################################################
### BEGIN OF STATE FILE
#####################################################

#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# Create a new 'Render View'
renderView1 = CreateView('RenderView')
renderView1.ViewSize = [1920,1080]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.OrientationAxesVisibility = 0
renderView1.StereoType = 0
renderView1.CameraPosition = [-0.06639579949199569, -3.497418118038352, 2.942109625083673]
renderView1.CameraViewUp = [0.9473537951210076, -0.21645985996474684, -0.2359362538777504]
renderView1.CameraParallelScale = 1.1830140069454795
renderView1.CameraParallelProjection = 1
renderView1.Background = [0.3686274509803922, 0.3686274509803922, 0.3686274509803922]

# ----------------------------------------------------------------
# BEGIN READERS
# ----------------------------------------------------------------
# create a new 'XDMF Reader'
surf = LegacyVTKReader(FileNames=ff)

# list of all sources
vs = [surf]

# time steps
vt = [np.array(s.TimestepValues) for s in vs]

# replace with ForceTime
surf = ForceTime(surf)

# all ForceTime
vft = [surf]
bvtk = surf

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

# ----------------------------------------------------------------
# END READERS
# ----------------------------------------------------------------

for i in list(range(len(ss))):
    fn = bo.format("{:04d}".format(ss[i]))
    if os.path.isfile(fn):
        Log("skip existing {:}".format(fn))
        continue

    with open(fn, "w") as f:
      f.write("")

    SetTime(i)

    Log("{:}/{:}: {:}".format(i + 1, len(ss), fn))
    SaveScreenshot(fn, renderView1)

exit(0)
