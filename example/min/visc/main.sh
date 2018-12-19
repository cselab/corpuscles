set -eu

. he.util
pi=3.141592653589793

v=0.41
#off=../../../data/oblate/laplace/Nt1280/oblate_volume_Vr0.5.off
off=data/2.off
ref=$off

Ka=1 Kga=1 Ks=0 Kh=0 Ke=0 Kb=1e-2 Kad=1 DA0D=50 C0=0

make
./main juelicher_xin \
 $v   $Ka $Kga $ref linear $Ks 0  $Kh $Ke   $Kb $C0 $Kad $DA0D < $off
