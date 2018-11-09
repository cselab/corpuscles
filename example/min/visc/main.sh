set -eu

. he.util
pi=3.141592653589793

v=0.5
#off=../../../data/prolate/laplace/Nt1280/prolate_volume_Vr1.off
off=$HOME/0.off
ref=$off

Ka=0.01 Kga=0 Ks=0 Kh=0 Ke=1e-9 Kb=1e-3 Kad=`ae "0.0/$pi*$Kb"` DA0D=0 C0=0
./main juelicher_xin $v   $Ka $Kga $ref linear $Ks 0  $Kh $Ke   $Kb $C0 $Kad $DA0D < $off
