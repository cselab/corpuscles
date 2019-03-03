set -eu

. co.util

make

Nt=$1; shift
off=$(he.path)/oblate/laplace/Nt${Nt}/oblate_area_Vr0.3.off

Da=2.15

pi=3.141592653589793115997964
D=0.000898798148042
A=$(ae 4*$pi)
Vr=0.31
Ka=0.5
Kga=1
Kv=5

dt=$1; shift
mu=$1; shift

Ke=0

Kb=0.001
C0=0
Kad=1
end=100000000
freq=100

DA0D=$(ae 8*$pi*$Da)
dir="Nt_${Nt}/dt_${dt}/mu_${mu}"

./main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $mu $dt $end $freq    $dir < $off
