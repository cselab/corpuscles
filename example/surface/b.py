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

av = sys.argv
ff = sorted(av[1:])

#####################################################
### BEGIN OF STATE FILE
#####################################################
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# Create a new 'Render View'
renderView1 = CreateView('RenderView')
renderView1.ViewSize = [1918, 1059]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.OrientationAxesVisibility = 0
renderView1.StereoType = 0
renderView1.CameraViewAngle = 40.0
renderView1.CameraPosition = [0, 0, 3]
renderView1.Background = [0.3686274509803922, 0.3686274509803922, 0.3686274509803922]

# ----------------------------------------------------------------
# BEGIN READERS
# ----------------------------------------------------------------
# create a new 'XDMF Reader'
surf = LegacyVTKReader(FileNames=ff[0])

# list of all sources
vs = [surf]

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
cellDatatoPointData1Display.ScalarOpacityUnitDistance = 0.0349909254054319
cellDatatoPointData1Display.ScalarOpacityFunction = cPWF

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
cellDatatoPointData2Display.ScalarOpacityUnitDistance = 0.0349909254054319
cellDatatoPointData2Display.ScalarOpacityFunction = separate_cellDatatoPointData2Display_cPWF

# set separate color map
cellDatatoPointData2Display.UseSeparateColorMap = True

fn = "o.png"
Log(fn)
SaveScreenshot(fn, renderView1)

exit(0)
