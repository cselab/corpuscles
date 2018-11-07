set -eu

. he.util
pi=3.141592653589793

off=$HOME/0.off
ref=$off

v=0.642
Ka=1 Kga=1 Kb=0 Ks=0 Kad=`ae 2/$pi*$Kb` DA0D=0 C0=0

./main juelicher $v   $Ka $Kga $ref linear $Ks 0      $Kb $C0 $Kad $DA0D < $off '>' q
