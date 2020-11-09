set -eu
pi=3.141592653589793115997964
A=12.5663706144
a3=-2
a4=8
b1=0.7
b2=0.75
C0=0.0
D=0.000898798148042
Da1=0.143
Kb=1.0
mub=139.260575205

alpha=$(echo $pi | gawk '{print 2.0/$1}')
Kad=$(echo $Kb $alpha | gawk '{print $1*$2}')
Ka=$(echo $mub | awk '{print $1*2}')

Kad=$(echo $Kb $alpha | gawk '{print $1*$2}')
Da0=$(echo $Da1 | awk '{print $1/100}')
DA0=$(echo $Kb $C0 $Kad $Da0 $D $pi $A | awk '{print $4*$7}')
DA0D=$(echo $DA0 $D | awk '{print $1/$2}')

co.eng juelicher_xin $Kb $C0 $Kad $DA0D < ini.off
co.eng strain ref.off lim $Ka $mub $a3 $a4 $b1 $b2 < ini.off
