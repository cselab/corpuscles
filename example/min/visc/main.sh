set -eu

. he.util
pi=3.141592653589793

v=0.56
off=../../../data/prolate/laplace/Nt1280/prolate_volume_Vr$v.off
ref=$off

Ka=1 Kga=1 Ks=0 Kh=0 Ke=0 Kb=1e-2 Kad=0 DA0D=0 C0=3.0
./main juelicher_xin $v   $Ka $Kga $ref linear $Ks 0  $Kh $Ke   $Kb $C0 $Kad $DA0D < $off
