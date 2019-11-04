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

paraview.simple._DisableFirstRenderCameraReset()
view = CreateView('RenderView')
view.ViewSize = [1918, 1059]
view.AxesGrid = 'GridAxes3DActor'
view.OrientationAxesVisibility = 0
view.StereoType = 0
view.CameraViewAngle = 40.0
view.CameraPosition = [0, 0, 3]
view.Background = [0.3686274509803922, 0.3686274509803922, 0.3686274509803922]

surf = LegacyVTKReader(FileNames=ff[0])
surf = ForceTime(surf)
surf = PointDatatoCellData(surf)
surf = CellDatatoPointData(surf)

Display2 = Show(surf, view)

# get separate color transfer function/color map for 'c'
separate_Display2_cLUT = GetColorTransferFunction('c', Display2, separate=True)
separate_Display2_cLUT.RGBPoints = [0.0, 1.0, 1.0, 1.0, 0.18421052396297455, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0]
separate_Display2_cLUT.ColorSpace = 'RGB'
separate_Display2_cLUT.NanColor = [1.0, 0.0, 0.0]
separate_Display2_cLUT.ScalarRangeInitialized = 1.0

separate_Display2_cPWF = GetOpacityTransferFunction('c', Display2, separate=True)
separate_Display2_cPWF.Points = [0.0, 0.0, 0.5, 0.0, 0.19298246502876282, 0.125, 0.5, 0.0, 0.4969879388809204, 0.11764705926179886, 0.5, 0.0, 1.0, 0.0, 0.5, 0.0]
separate_Display2_cPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
Display2.Representation = 'Volume'
Display2.ColorArrayName = ['POINTS', 'c']
Display2.LookupTable = separate_Display2_cLUT
Display2.ScalarOpacityUnitDistance = 0.0349909254054319
Display2.ScalarOpacityFunction = separate_Display2_cPWF
Display2.UseSeparateColorMap = True

SaveScreenshot("b.png", view, TransparentBackground = 1)
Log("b.png")

exit(0)
