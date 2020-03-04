# Example

     ./roi -r 974 974 191   1074 1074 291 /u/a.tif > a.vtk
     ./roi -r 999 999 216   1049 1049 266 /u/a.tif > b.vtk
     ./crop -r 974 974 191  1074 1074 192 -b -o box.tif /u/a.tif
     ./crop -r 974 974 191  1074 1074 192 -o crop.tif /u/a.tif
     ./crop -r 974 1174 191  1074 1274 192 -b -o box.tif /u/a.tif
     ./roi -r 974 1174 170  1074 1274 220  /u/a.tif >box.vtk
