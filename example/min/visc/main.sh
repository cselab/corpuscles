set -eu

. he.util
pi=3.141592653589793

off=$HOME/0.off
#off=$HOME/498.off
ref=$off

v=0.45
Ka=5 Kga=0.5 Ks=0 Kb=0.01 Kad=`ae "10/$pi*$Kb"` DA0D=50 C0=0

./main juelicher $v   $Ka $Kga $ref skalak $Ks 0      $Kb $C0 $Kad $DA0D < $off
