set -eu

Kb=0.01 C0=0 Kad=0 DA0D=0
Vr=0.25
freq=500
end=200000

off=$(he.path)/sph/laplace/Nt1280.off
make

he.run ./voronoi gompper_xin $Vr 1 2 1 0 $Kb $C0 $Kad $DA0D $end $freq < $off > Vr$Vr.msg
