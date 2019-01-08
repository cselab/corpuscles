set -eu

. he.util

make

fa=0.01
pi=3.141592653589793115997964

Vr=0.642
Ka=$(ae 0.5*$fa)
Kga=$(ae 1*$fa)
Kv=$(ae 5*$fa)
Ke=0

Kb=$(ae 0.001*$fa)
C0=0
Kad=$(ae 2*$Kb/$pi)
D=0.000898798148042

A=$(ae 4*$pi)
off=data/Nt1280_01.off
end=1000000
freq=200

Da=0.3
Da=$(ae $Da/100)
DA0D=$(ae "($Da - $Kb*$D*$C0/$pi/$Kad)*$A/$D")
    
echo ./main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off # > msg
exit
