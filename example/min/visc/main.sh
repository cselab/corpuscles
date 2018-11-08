set -eu

. he.util
pi=3.141592653589793

v=0.5
off=../../../data/prolate/laplace/Nt1280/prolate_volume_Vr0.8.off
#off=$HOME/q.off
ref=$off

Ka=5 Kga=0.5 Ks=0 Kh=0 Kb=0.025 Kad=`ae "0.0/$pi*$Kb"` DA0D=0 C0=0

./main juelicher_xin $v   $Ka $Kga $ref linear $Ks 0  $Kh    $Kb $C0 $Kad $DA0D < $off
