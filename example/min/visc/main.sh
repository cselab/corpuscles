set -eu

. he.util
pi=3.141592653589793

off=$HOME/e.off
ref=$HOME/s.off

v=0.642
Ka=5 Kga=0.5 Ks=0 Kb=0.00001 Kad=`ae "2/$pi*$Kb"` DA0D=150 C0=0

./main juelicher $v   $Ka $Kga $ref linear $Ks 0      $Kb $C0 $Kad $DA0D < $off
