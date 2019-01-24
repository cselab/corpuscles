set -eu

. he.util

make

# Nt=5120
off=$(he.path)/sph/laplace/0.off
ref=$(he.path)/sph/laplace/0.off

Da=0.3

pi=3.141592653589793115997964
D=0.000898798148042
Vr=0.642
Ka=0.5
Kga=1
Kv=5

Kas=0
mus=100

dt=0.01
mu=10

Kb=0.001
C0=0
Kad=0
end=100000000
freq=100

DA0D=0

dir=o

./main juelicher_xin $Vr $Ka $Kga $Kv $Kb $C0 $Kad $DA0D $mu $dt $end $freq    $dir  \
       strain $ref lim $Kas $mus 0 0 0 0 < $off
