set -eu

Kb=0.01
C0=0
Kad=0
DA0D=0
end=200000
freq=500
Vr=0.4

# off=$(he.path)/sph/laplace/Nt1280.off
off=start/main.off
#off=~/hex.off
make

he.run ./voronoi meyer_xin $Vr 10 2 1 0 $Kb $C0 $Kad $DA0D $end $freq < $off > Vr$Vr.msg
