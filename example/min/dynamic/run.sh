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
DA0D=0
D=0.000898798148042

A=$(ae 4*$pi)
off=data/Nt1280_01.off
end=1000000
freq=200
Da1=0.3
Da0=$(echo $Da1 | awk '{print $1/100}')
DA0=$(echo $Kb, $C0, $Kad, $Da0, $D, $pi, $A | awk '{print ($4 - $1*$5*$2/$6/$3)*$7}')
DA0D=$(echo $DA0, $D | awk '{print $1/$2}')
    
echo ./main juelicher_xin $Vr $Ka $Kga $Kv $Ke $Kb $C0 $Kad $DA0D $end $freq < $off # > msg
exit
