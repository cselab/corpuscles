set -eu

. he.util

make

#off=data/Nt1280_01.off
off=data/Nt5120_01.off

Da=0.3

f=0.1
pi=3.141592653589793115997964
D=0.000898798148042
A=$(ae 4*$pi)
Vr=0.642
Ka=$(ae 0.5*$f)
Kga=$(ae 1*$f)
Kv=$(ae 5*$f)
mu=$(ae 1000*$f)
Ke=0

Kb=$(ae 0.001*$f)
C0=0
Kad=$(ae 2*$Kb/$pi)
end=100000000
freq=2000

Da=$(ae $Da/100)
DA0D=$(ae "($Da - $Kb*$D*$C0/$pi/$Kad)*$A/$D")
    
./main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $mu $end $freq < $off
