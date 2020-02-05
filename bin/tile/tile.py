#!/usr/bin/env python

import sys
import subprocess as sp

usg = '''tile list of movies
./tile.py
'''
av = sys.argv
m = 1

rate = av[m]; m += 1
rx = int(av[m]); m += 1
sx = int(av[m]); m += 1
sy = int(av[m]); m += 1

if len(av) <= 1:
    sys.stderr.write(usg)
    exit(1)
nx = int(av[m]); m += 1
ny = int(av[m]); m += 1

ry = rx*sy*ny//(sx*nx)
n = nx * ny

vs = []
for k in range(n):
    vs.append(av[m]); m += 1

out = av[m]; m += 1

# output resolution

# tiles
sx = rx // nx
sy = ry // ny

o = "ffmpeg"
for k in range(n):
    o += " -i {:}".format(vs[k])

dx = 0

o += " -filter_complex 'nullsrc=size={:}x{:} [base]; \n".format(rx, ry)
for k in range(n):
    o += '''[{k}:v] setpts=PTS-STARTPTS [b{k}]; [b{k}] crop={sx}:{sy}:{dx} [b{k}] ;
'''.format(**locals())

#, scale={ix}x{iy}

pr = "base"
for j in range(ny):
    for i in range(nx):
        k = j * nx + i
        b = "b{:}".format(k)
        t = "t{:}".format(k)
        o += "[{:}][{:}] overlay=shortest=1:x={:}:y={:}".format(
                pr, b, sx * i, sy * j)
        if k + 1 < n:
            o += " [{:}]; ".format(t)
        pr = t
o += "' -loglevel error -y -r {:} -c:v libx264 {:}".format(rate, out)

print(o)
#sp.check_call(o, shell=True)
