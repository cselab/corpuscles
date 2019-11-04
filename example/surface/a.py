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
Display1 = Show(surf, view)
cLUT = GetColorTransferFunction('c')
cLUT.RGBPoints = [0, 1, 1, 1,       1, 1, 1, 1]
cLUT.ScalarRangeInitialized = 1.0
cPWF = GetOpacityTransferFunction('c')
midpoint = 0.5
sharpness = 0
cPWF.Points = [0, 0, midpoint, sharpness,     1, 0.01, midpoint, sharpness]
cPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
Display1.Representation = 'Volume'
Display1.ColorArrayName = ['POINTS', 'c']
Display1.LookupTable = cLUT
Display1.ScalarOpacityUnitDistance = 0.01
Display1.ScalarOpacityFunction = cPWF

SaveScreenshot("a.png", view, TransparentBackground = 1)
Log("a.png")

exit(0)
