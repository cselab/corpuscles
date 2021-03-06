set -eu

Kb=0.01 C0=0 Kad=0 DA0D=0
Vr=0.4
freq=500
end=99999999

#off=start/main.off
off=$(co.path)/sph/laplace/Nt1280.off
make

co.run ./voronoi meyer_xin $Vr 1 2 2 0 \
       $Kb $C0 $Kad $DA0D $end $freq < $off > Vr$Vr.msg
