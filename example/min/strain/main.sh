set -eu

. he.util

make

off=$(he.path)/prolate/laplace/Nt1280/prolate_volume_Vr0.64.off
ref=$(he.path)/sph/laplace/0.off

pi=3.141592653589793115997964
Vr=0.642
Ka=1
Kga=1
Kv=1

Kas=0
mus=0

dt=0.001
mu=100

Kb=0.01

C0=0
Kad=0
end=100000000
freq=100

DA0D=0

dir=o

he.run ./main juelicher_xin $Vr $Ka $Kga $Kv $Kb $C0 $Kad $DA0D $mu $dt $end $freq    $dir  \
       strain $ref lim $Kas $mus 0 0 0 0 < $off
