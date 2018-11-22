
Ka=0.5
Kga=1
Kv=3
Ke=0
Kb=0.002
C0=0
Kad=0
DA0D=0
Vstart=1.0
Vend=0.5
numVr=8
stepVr=20000
freq=200

off=../../../../data/sph/laplace/Nt1280.off

../main juelicher_xin $Ka $Kga $Kv $Ke  $Kb $C0 $Kad $DA0D $Vstart $Vend $numVr $stepVr $freq < $off > shrink.msg
