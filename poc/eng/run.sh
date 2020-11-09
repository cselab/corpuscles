. co.util

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

alpha=$(ae 2.0/$pi)
Kad=$(ae $Kb*$alpha)
Ka=$(ae 2*$mub)

Kad=$(ae $Kb*$alpha)
Da0=$(ae $Da1/100)
DA0=$(ae $Da0*$A)
DA0D=$(ae $DA0/$D)

co.eng juelicher_xin $Kb 0 0 0 < ini.off
co.eng juelicher_xin $Kb $C0 $Kad $DA0D < ini.off
co.eng strain ref.off lim $Ka $mub $a3 $a4 $b1 $b2 < ini.off
