set -eu

Kb=0.01
C0=0
Kad=0
DA0D=0
end=200000
freq=500
Vr=0.4

off=$(he.path)/sph/laplace/Nt1280.off
make

./main meyer_xin $Vr 1 2 1 0 $Kb $C0 $Kad $DA0D $end $freq < $off > Vr$Vr.msg
