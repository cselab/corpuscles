set -eu

. he.util

make

# Nt=5120
off=$(he.path)/sph/laplace/0.off
ref=$(he.path)/sph/laplace/0.off

Da=0.3

pi=3.141592653589793115997964
D=0.000898798148042
A=$(ae 4*$pi)
Vr=0.642
Ka=0.5
Kga=1
Kv=5

dt=0.01
mu=10

Ke=0

Kb=0.001
C0=0
Kad=$(ae 2*$Kb/$pi)
end=100000000
freq=10000

Da=$(ae $Da/100)
DA0D=$(ae "($Da - $Kb*$D*$C0/$pi/$Kad)*$A/$D")

dir=o

./main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $mu $dt $end $freq    $dir  strain $ref lim 1 2 3 4 5 6 < $off
